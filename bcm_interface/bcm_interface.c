/******************************************************************************/
/*! @file       bcm_interface.c
    @brief      This program create(& kill) child process(bcm2835_for_java)
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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "bcm_interface.h"

/// \defgroup bcm_interface bcm_interface
/// \ingroup PARENT
/// @{
int pid = 0;
int child;    ///< pid of bcm2835_for_java
int fd;       ///< file discripter of shared memory

//int local_sync_code;  ///< base of sync code

int bi_status;    ///< status of bcm_interface

/// \cond 
void do_child( char *args )
{
    char *new_argv[3];
    new_argv[0] = "../bcm2835_for_java/bcm2835_for_java";
    new_argv[1] = args;
    new_argv[2] = NULL;
    execv(new_argv[0], new_argv );
    printf("execv failed %s %s\n",new_argv[0],new_argv[1]);
    return;
}
/// \endcond 

/// Create shared memory, setup ring_buff, fork child process and call bcm2835_init().
/// \param[in] args   shared file name like as "/tmp/shm"
/// \par            Refer
/// \par            Modify
int bi_init( char *args )
{
char *s;

    if( args == NULL )
    {
        printf("bcm_interface needs 1 parameter like as /tmp/shared_mem\n");
      exit ( -1 );
    }
    else
    {
      fd = open( args , O_RDWR);
      if( fd == -1 )
      {
        printf("file %s can't open\n",args);
        exit(-1);
      }
      else
      {
         s = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
         if( s == MAP_FAILED )
         {
            printf("we can't create mapping file\n");
            exit(-1);
         }
      }
    }

    w_buff = (struct ring_buff *)s;
    r_buff = (struct ring_buff *)(s+sizeof(struct ring_buff) );
    bi_send_buff = (char *)( s + 2*sizeof(struct ring_buff) );
    bi_rec_buff = (char *)( bi_send_buff + TEMP_BUFF_SIZE );
    init_ring_buff(w_buff,WRITER_INIT);
    init_ring_buff(r_buff,READER_INIT);

    sync_code = (int *)( bi_rec_buff + TEMP_BUFF_SIZE );
    reply_code = (int *)( sync_code + 1 );
//    local_sync_code = 0;
    *sync_code = 0;
    *reply_code = 0;
  
    child = fork();
    if( child < 0 )
    {
       printf("we can't create child\n");
    }
    else if( child == 0 )
    {
        do_child(args);
        exit(0);
    }

    printf("bcm_interface ver1.01a start \n");
    if( context_switch(100) != 0 )
    {
        printf("bi_status = %d  @bi_init \n",bi_status );
        return -1;
    }
    else if( ope_hello("Hello child!") != 0 )
    {
        printf("Ring buffer failed bi_status=%d\n",bi_status );
        return -1;
    }

    return ope_init();    // bcm2835_init()
}

/// Call bcm2835_init().
/// \param[in] args   shared file name like as "/tmp/shm"
/// \par            Refer
/// \par            Modify
int bcm_init( void )
{
    return ope_init();    // bcm2835_init()
}

/// Call bcm2835_close(), Delete shared memory and kill child process.
/// \par            Refer
/// \par            Modify
int bi_close(void)
{

  ope_close();     // bcm2835_close())
    if( ope_exit() != 0 )
    {
        printf("bi_status = %d  @bi_close \n",bi_status );
        return -1;
    }
    close( fd );
    kill(child,SIGINT);
    printf("Close bcm_interface\n");
    return 0;
}

/// Call bcm2835_close()
/// \par            Refer
/// \par            Modify
int bcm_close(void)
{
    return ope_close();    // bcm2835_close())
}

/// @}

