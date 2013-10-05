/******************************************************************************/
/*! @file       bi_common.c
    @brief      This program is common library which handle local buff and ring_buff
*******************************************************************************
 * 
 * 
 * 
*******************************************************************************
    @date       2013/09/18
    @author     Akira Hiramine
    @par        Revision
    $Id$
    @par        Copyright
    2013 Denshikobo-Life,Ltd. All rights reserved.
*******************************************************************************
    @par        History
    - 2013/09/18 Akira Hiramine
      -# Initial Version
    - 2013/09/30 Akira Hiramine
      -# Delete the useless codes
******************************************************************************/

/** INCLUDES *******************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "bi_common.h"

/** VARIABLES ********************************************************/
char buff[RING_BUFF_SIZE]; ///< local buffer
int wp; ///< write pointer on local buffer
int rp; ///< read pointer on local buffer
union int_char u_ic;    ///< convert between int and char[]
union long_char u_lc;   ///< convert between long and char[]
int bi_status;          ///< bcm interface status STATUS_FINE|TIME_OUT

struct ring_buff *r_buff;    ///< ring buffer pointer on shared memoory(for receive)
struct ring_buff *w_buff;    ///< ring buffer pointer on shared memoory(dor send))
char *bi_send_buff;          ///< byte[] on shared memoory send from Java to BCN 
char *bi_rec_buff;           ///< byte[] on shared memoory send from BCM to JAVA 
volatile int *sync_code;        ///< ope_sync referece data 
volatile int *reply_code;       ///< ope_sync reply data

/// \addtogroup COMMON
/// @{

/// calc data size on ring buffer
/// \param[in] rb struct ring_buff*
/// \par            Refer
///         - rb->wp
///         - rb->rp
/// \par            Modify
/// \ return         done:0, time_over:-1
int calc_data_size( struct ring_buff *rb)
{
    int r,w;
    r = rb->rp;
    w = rb->wp;
    if( w < r )w += RING_BUFF_SIZE;
    return (w - r);
}

/// calc rest size on ring buffer
/// \param[in] rb struct ring_buff*
/// \par            Refer
///         - rb->wp
///         - rb->rp
/// \par            Modify
/// \ return         done:0, time_over:-1
int calc_rest_size( struct ring_buff *rb)
{
    return (RING_BUFF_SIZE - calc_data_size(rb));
}

/// copy local buff to shared ring_buff
/// \param[in] rb struct ring_buff*
/// \param[in] str char*
/// \param[in] len int
/// \par            Refer
///         - w_buff
///         - buff
///         - wp
/// \par            Modify
///         - w_buff->wp
/// \ return         done:0, time_over:-1
/// copy local buff to shared ring_buff
/// \param[in] rb struct ring_buff*
/// \par            Modify
///        - w_buff->wp
/// \return         done:0, rest size smaller then len:-1
int put_ring_buff(struct ring_buff *rb, char *str, int len )
{
    int i;
    int w;

    if( len >= calc_rest_size( rb ) )
    {
        return -1;
    }
    else
    {
      w = rb->wp;
      for( i = 0; i<len;i++)
      {
        rb->buff[w++] = *(str+i);
        w &= RING_POINTER_MASK;
      }
      rb->wp = w;
      return 0;
    }
}

/// copy shared ring_buff to local buffer
/// \param[in] rb struct ring_buff*
/// \param[out] buff char *
/// \param[in] max_len size of buff
/// \par            Refer
/// \par            Modify
/// \ return         done:data error:-1
int get_ring_buff(struct ring_buff *rb, char *buff, int max_len)
{
    int r;
    int w;
    int len;

    r = rb->rp;
    w = rb->wp;
    len = 0;
    while( r != w )
    {
      if( len >= max_len)break;
      buff[len++] = rb->buff[r++];
      r &= RING_POINTER_MASK;
    }
    rb->rp = r;
    return len;
}

/// initialize shared ring_buff
/// \param[in] rb struct ring_buff*
/// \param[in] init_type READER_INIT|:WRITER_INIT:BOTH_INIT
/// \par            Refer
/// \par            Modify
/// \ return         done:data error:-1
void init_ring_buff(struct ring_buff *rb, int init_type)
{
    switch(init_type)
    {
        case READER_INIT:
            rb->rp = 0;
            break;
        case WRITER_INIT:
            rb->wp = 0;
            break;
        case BOTH_INIT:
        default:
            rb->rp = 0;
            rb->wp = 0;
            break;
    }
}

/// reset wp & set ope code
/// \param[in] ope_code unsigned char
/// \par            Refer
/// \par            Modify
///         - buff[0]
///         - wp
void set_ope_code( unsigned char ope_code)
{
    wp = 0;
    buff[wp++] = ope_code;
}

/// set byte code & increment wp
/// \param[in] code unsigned char
/// \par            Refer
/// \par            Modify
///         - buff
///         - wp
void set_byte_code( unsigned char code)
{
    buff[wp++] = code;
}

/// set short int code & increment wp
/// \param[in] code short int
/// \par            Refer
/// \par            Modify
///         - buff[0]
///         - wp
void set_short_code( short int code )
{
    u_ic.s[0] = code;
    buff[wp++] = u_ic.c[0];
    buff[wp++] = u_ic.c[1];
}

/// set int code & increment wp
/// \param[in] code int
/// \par            Refer
/// \par            Modify
///         - buff[0]
///         - wp
void set_int_code( int code)
{
    u_ic.i = code;
    buff[wp++] = u_ic.c[0];
    buff[wp++] = u_ic.c[1];
    buff[wp++] = u_ic.c[2];
    buff[wp++] = u_ic.c[3];
}

/// set long code & increment wp
/// \param[in] code long
/// \par            Refer
/// \par            Modify
///         - buff[0]
///         - wp
void set_long_code( long code)
{
    u_lc.l = code;
    set_int_code( u_lc.i[0]);
    set_int_code( u_lc.i[1]);
}

/// reset rp & get ope code
/// \par            Refer
///         - buff[0]
/// \par            Modify
///         - rp
void get_ope_code( void )
{
    rp = 0;
    rp += get_ring_buff( r_buff, buff, 1 );
}

/// get byte code & increment rp
/// \par            Refer
/// \par            Modify
///         - buff
///         - rp
void get_byte_code( void )
{
    rp += get_ring_buff( r_buff, buff+rp, 1 );
}

/// get short int code & increment rp
/// \par            Refer
///         - buff
/// \par            Modify
///         - rp
void get_short_code( void )
{
    rp += get_ring_buff( r_buff, buff+rp, 2 );
}

/// get int code & increment rp
/// \par            Refer
///         - buff
/// \par            Modify
///         - rp
void get_int_code( void )
{
    rp += get_ring_buff( r_buff, buff+rp, 4 );
}

/// get long code & increment rp
/// \par            Refer
///         - buff
/// \par            Modify
///         - rp
void get_long_code( void )
{
    rp += get_ring_buff( r_buff, buff+rp, 8 );
}

/// copy byte data to dest from src
/// \param[out] dest char*
/// \param[in] src char*
/// \param[in] len size of byte
/// \par            Refer
/// \par            Modify
void copy_str( char *dest, char* src, int len)
{
int i;
   for( i=0; i<(len+1);i++ )
   {
      *(dest+i) = *(src+i);
   }
}

/// \cond 
void dump_buff(void)
{
    int i;
    printf("wp=%d rp=%d ",wp,rp);
    for( i=0;i<wp;i++)
    {
      printf("%02x ",buff[i]);
    }
    printf("  \n");
}
/// \endcond 

/// context switch
/// \param[in] max_count
/// \par            Refer
///         - w_buff
/// \par            Modify
///         - bi_status
int context_switch( int max_count)
{
    int loop_count;
    loop_count = 0;
    while( calc_data_size( w_buff ) != 0 )
    {
        usleep(1000);
        if( (loop_count++) >= max_count )
        {
            bi_status = COUNT_OVER;
            return -1;
        }
    }
    return 0;
}
/// @}

/** EOF bi_common.c *************************************************/


