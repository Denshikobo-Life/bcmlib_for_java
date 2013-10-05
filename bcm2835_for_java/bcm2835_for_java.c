/******************************************************************************/
/*! \addtogroup PARENT
    @file       bcm2835_for_java.c
    @brief      This program is Interface between bcm2835 library and shared ring buffer
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
      -# Delete The useless code
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <termios.h>		//Used for UART
#include "bcm2835_for_java.h"

int bi_status;  ///< bcm_interface status STATUS_FINE|TIME_OUT|elss ?

/// \defgroup bcm2835_for_java bcm2835_for_java 
/// \ingroup CHILD
/// @{

/// Copy local buff to shared ring_buff
/// \par            Refer
/// \par            Modify
void put_reply(void)
{
    while( put_ring_buff( w_buff, buff, wp ) != 0 )
    {
        if( context_switch(100) != 0 )
          {
            printf("rttot @put_reply\n");
          }
    }
}

/// Mark reply_code
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void mark_sync(void)
{
    *reply_code = *sync_code;
}

// int bcm2835_init(void);
/// Call bcm2835_init() & set ope_code & return code into local buff
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void bcm_init(void)
{
    set_ope_code( OPE_INIT );
    set_int_code( bcm2835_init() );
    put_reply();
    mark_sync();
}

//int bcm2835_close(void);
/// Call bcm2835_close() and set ope_code & return code into local buff
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void bcm_close(void)
{
    set_ope_code( OPE_CLOSE );
    set_int_code( bcm2835_close() );
    put_reply();
    mark_sync();
}

//void  bcm2835_set_debug(uint8_t d);
/// Call bcm2835_set_debug()
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void ope_set_debug(void)
{
    get_byte_code();
    bcm2835_set_debug( *((uint8_t *)(buff+1)) );
}

//uint32_t bcm2835_peri_read(volatile uint32_t* paddr);
/// Call bcm2835_peri_read() with 1 parameter and set ope_code & return code on local buff 
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void ope_peri_read(void)
{
uint32_t ret;
    get_int_code();
    ret = bcm2835_peri_read( (volatile uint32_t *)(buff+5) );   // buff:ope(1),ret(4),data
    set_ope_code( OPE_PERI_READ );
    set_int_code( ret );
    put_reply();
    mark_sync();
}

//uint32_t bcm2835_peri_read_nb(volatile uint32_t* paddr);
/// Call bcm2835_peri_read_nb() with 1 parameter and set ope_code & return code on local buff 
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
void ope_peri_read_nb(void)
{
    uint32_t ret;
    get_int_code();
    ret = bcm2835_peri_read( (volatile uint32_t *)(buff+5) );
    set_ope_code( OPE_PERI_READ_NB );
    set_int_code( ret );
    put_reply();
    mark_sync();
}

//void bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value);
/// Call bcm2835_peri_write() with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_peri_write(void)
{
volatile uint32_t* paddr;
uint32_t value;
    get_int_code();
    get_int_code();
    paddr = *((volatile uint32_t **)(buff+1));
    value = *((volatile uint32_t *)(buff+5));
    bcm2835_peri_write( paddr, value );
//    set_ope_code( OPE_PERI_WRITE );
}

//void bcm2835_peri_write_nb(volatile uint32_t* paddr, uint32_t value);
/// Call bcm2835_peri_write_nb with 2 parameter
/// \par            Refer
/// \par            Modify
void ope_peri_write_nb(void)
{
volatile uint32_t* paddr;
uint32_t value;
    get_int_code();
    get_int_code();
    paddr = *((volatile uint32_t **)(buff+1));
    value = *((volatile uint32_t *)(buff+5));
    bcm2835_peri_write_nb( paddr, value );
}

//void bcm2835_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask);
/// Call bcm2835_peri_set_bits with 3 parameter
/// \par            Refer
/// \par            Modify
void ope_peri_set_bits(void)
{
volatile uint32_t* paddr;
uint32_t value;
uint32_t mask;
    get_int_code();
    get_int_code();
    get_int_code();
    paddr = *((volatile uint32_t **)(buff+1));
    value = *((volatile uint32_t *)(buff+5));
    mask = *((volatile uint32_t *)(buff+9));
    bcm2835_peri_set_bits( paddr, value, mask );
}

//void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode);
/// Call bcm2835_gpio_fsel with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_gpio_fsel(void)
{
    get_byte_code();
    get_byte_code();
    bcm2835_gpio_fsel( *((uint8_t *)(buff+1)), *((uint8_t *)(buff+2)) );
}

//void bcm2835_gpio_set(uint8_t pin);
/// Call bcm2835_gpio_set with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_set(void)
{
    get_byte_code();
    bcm2835_gpio_set( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr(uint8_t pin);
/// Call bcm2835_gpio_clr with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr(void)
{
    get_byte_code();
    bcm2835_gpio_clr( *((uint8_t *)(buff+1)) );
//   set_ope_code( OPE_GPIO_CLR );
}

//void bcm2835_gpio_set_multi(uint32_t mask);
/// Call bcm2835_gpio_set_multi with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_set_multi(void)
{
    get_byte_code();
    bcm2835_gpio_set_multi( *((uint8_t *)(buff+1)) );
//    set_ope_code( OPE_GPIO_SET_MULTI );
}

//void bcm2835_gpio_clr_multi(uint32_t mask);
/// Call bcm2835_gpio_clr_multi with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_multi(void)
{
    get_int_code();
    bcm2835_gpio_clr_multi( *((uint32_t *)(buff+1)) );
//    set_ope_code( OPE_GPIO_CLR_MULTI );
}

//uint8_t bcm2835_gpio_lev(uint8_t pin);
/// Call bcm2835_gpio_lev with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_lev(void)
{
uint8_t ret;
    get_byte_code();
    ret = bcm2835_gpio_lev( *((uint8_t *)(buff+1)) );
    set_ope_code( OPE_GPIO_LEV );
    set_byte_code( ret );
    put_reply();
    mark_sync();
}

//uint8_t bcm2835_gpio_eds(uint8_t pin);
/// Call bcm2835_gpio_eds with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_eds(void)
{
uint8_t ret;
    get_byte_code();
    ret = bcm2835_gpio_eds( *((uint8_t *)(buff+1)) );
    set_ope_code( OPE_GPIO_EDS );
    set_byte_code( ret );
    put_reply();
    mark_sync();
}

//void bcm2835_gpio_set_eds(uint8_t pin);
/// Call bcm2835_gpio_set_eds with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_set_eds(void)
{
    get_byte_code();
    bcm2835_gpio_lev( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_ren(uint8_t pin);
/// Call bcm2835_gpio_ren with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_ren(void)
{
    get_byte_code();
    bcm2835_gpio_ren( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_ren(uint8_t pin);
/// Call bcm2835_gpio_clr_ren with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_ren(void)
{
    get_byte_code();
    bcm2835_gpio_clr_ren( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_fen(uint8_t pin);
/// Call bcm2835_gpio_fen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_fen(void)
{
    get_byte_code();
    bcm2835_gpio_fen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_fen(uint8_t pin);
/// Call bcm2835_gpio_clr_fen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_fen(void)
{
    get_byte_code();
    bcm2835_gpio_clr_fen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_hen(uint8_t pin);
/// Call bcm2835_gpio_hen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_hen(void)
{
    get_byte_code();
    bcm2835_gpio_hen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_hen(uint8_t pin);
/// Call bcm2835_gpio_clr_hen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_hen(void)
{
    get_byte_code();
    bcm2835_gpio_clr_hen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_len(uint8_t pin);
/// Call bcm2835_gpio_len with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_len(void)
{
    get_byte_code();
    bcm2835_gpio_len( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_len(uint8_t pin);
/// Call bcm2835_gpio_clr_len with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_len(void)
{
    get_byte_code();
    bcm2835_gpio_clr_len( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_aren(uint8_t pin);
/// Call bcm2835_gpio_aren with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_aren(void)
{
    get_byte_code();
    bcm2835_gpio_aren( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_aren(uint8_t pin);
/// Call bcm2835_gpio_clr_aren with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_aren(void)
{
    get_byte_code();
    bcm2835_gpio_clr_aren( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_afen(uint8_t pin);
/// Call bcm2835_gpio_afen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_afen(void)
{
    get_byte_code();
    bcm2835_gpio_afen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_clr_afen(uint8_t pin);
/// Call bcm2835_gpio_clr_afen with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_clr_afen(void)
{
    get_byte_code();
    bcm2835_gpio_clr_afen( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_pud(uint8_t pud);
/// Call bcm2835_gpio_pud with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_pud(void)
{
    get_byte_code();
    bcm2835_gpio_pud( *((uint8_t *)(buff+1)) );
}

//void bcm2835_gpio_pudclk(uint8_t pin, uint8_t on);
/// Call bcm2835_gpio_pudclk with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_pudclk(void)
{
    get_byte_code();
    get_byte_code();
    bcm2835_gpio_pudclk( *((uint8_t *)(buff+1)), *((uint8_t *)(buff+2)) );
}

//void bcm2835_gpio_write(uint8_t pin, uint8_t on);
/// Call bcm2835_gpio_write with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_write(void)
{
    get_byte_code();
    get_byte_code();
    bcm2835_gpio_write( *((uint8_t *)(buff+1)), *((uint8_t *)(buff+2)) );
}

//uint32_t bcm2835_gpio_pad(uint8_t group);
/// Call bcm2835_gpio_pad with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_pad(void)
{
uint32_t ret;
    get_byte_code();
    ret = bcm2835_gpio_pad( *((uint8_t *)(buff+1)) );
    set_ope_code( OPE_GPIO_PAD );
    set_int_code( ret );
    put_reply();
    mark_sync();
}

//void bcm2835_gpio_set_pad(uint8_t group, uint32_t control);
/// Call bcm2835_gpio_set_pad with 2 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_set_pad(void)
{
    get_byte_code();
    get_int_code();
    bcm2835_gpio_set_pad( *((uint8_t *)(buff+1)), *((uint32_t *)(buff+2)) );
}

//void bcm2835_delay (unsigned int millis);
/// Call bcm2835_delay with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_delay(void)
{
    get_int_code();
    bcm2835_delay( *((uint32_t *)(buff+1)) );
}

//void bcm2835_delayMicroseconds (uint64_t micros);
/// Call bcm2835_delaymicroseconds with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_delaymicroseconds(void)
{
    get_long_code();
    bcm2835_delayMicroseconds( *((uint64_t *)(buff+1)) );
}

//void bcm2835_gpio_write_multi(uint32_t mask, uint8_t on);
/// Call bcm2835_gpio_write_multi with 2 parameter
/// \par            Refer
/// \par            Modify
void ope_gpio_write_multi(void)
{
    get_int_code();
    get_byte_code();
    bcm2835_gpio_write_multi( *((uint32_t *)(buff+1)), *((uint8_t *)(buff+5)) );
}

//void bcm2835_gpio_write_mask(uint32_t value, uint32_t mask);
/// Call bcm2835_gpio_write_mask with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_gpio_write_mask(void)
{
    get_int_code();
    get_int_code();
    bcm2835_gpio_write_mask( *((uint32_t *)(buff+1)), *((uint32_t *)(buff+5)) );
}

//void bcm2835_gpio_set_pud(uint8_t pin, uint8_t pud);
/// Call bcm2835_gpio_set_pud with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_gpio_set_pud(void)
{
    get_byte_code();
    get_byte_code();
    bcm2835_gpio_set_pud( *((uint8_t *)(buff+1)), *((uint8_t *)(buff+2)) );
}

//void bcm2835_spi_begin(void);
/// Call bcm2835_spi_begin
/// \par            Refer
/// \par            Modify
void ope_spi_begin(void)
{
    bcm2835_spi_begin();
}

//void bcm2835_spi_end(void);
/// Call bcm2835_spi_end
/// \par            Refer
/// \par            Modify
void ope_spi_end(void)
{
    bcm2835_spi_end();
}

//void bcm2835_spi_setBitOrder(uint8_t order);
/// Call bcm2835_spi_setBitOrder with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_setbitorder(void)
{
    get_byte_code();
    bcm2835_spi_setBitOrder( *((uint8_t *)(buff+1)) );
}

//void bcm2835_spi_setClockDivider(uint16_t divider);
/// Call bcm2835_spi_setClockDivider with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_setclockdivider(void)
{
    get_short_code();
    bcm2835_spi_setClockDivider( *((uint16_t *)(buff+1)) );
}

//void bcm2835_spi_setDataMode(uint8_t mode);
/// Call bcm2835_spi_setDataMode with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_setdatamode(void)
{
    get_byte_code();
    bcm2835_spi_setDataMode( *((uint8_t *)(buff+1)) );
}

//void bcm2835_spi_chipSelect(uint8_t cs);
/// Call bcm2835_spi_chipSelect with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_chipselect(void)
{
    get_byte_code();
    bcm2835_spi_chipSelect( *((uint8_t *)(buff+1)) );
}

//void bcm2835_spi_setChipSelectPolarity(uint8_t cs, uint8_t active);
/// Call bcm2835_spi_setChipSelectPolarity with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_setchipselectpolarity(void)
{
    get_byte_code();
    get_byte_code();
    bcm2835_spi_setChipSelectPolarity( *((uint8_t *)(buff+1)), *((uint8_t *)(buff+2)) );
}

//uint8_t bcm2835_spi_transfer(uint8_t value);
/// Call bcm2835_spi_transfer with 1 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_transfer(void)
{
uint8_t ret;
    get_byte_code();
    ret = bcm2835_spi_transfer( *((uint8_t *)(buff+1)) );
    set_ope_code( OPE_SPI_TRANSFER );
    set_byte_code( ret );
    put_reply();
    mark_sync();
}

//void bcm2835_spi_transfernb(char* tbuf, char* rbuf, uint32_t len);
/// Call bcm2835_spi_transfernb with 3 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_transfernb(void)
{
char *tbuf;
char *rbuf;
uint32_t len;
    get_int_code();
    get_int_code();
    get_int_code();
    tbuf = *(char **)(buff+1);
    rbuf = *(char **)(buff+5);
    len = *((uint32_t *)(buff+9));
//  printf("B nb tbuf=%p rbuf=%p len=%d \n",tbuf,rbuf,len);
  bcm2835_spi_transfernb( bi_send_buff, bi_rec_buff, len );
    set_ope_code( OPE_SPI_TRANSFERNB );
    set_int_code( (int)rbuf );
    set_int_code( len );
//dump_buff();
//printf("w_buff=%p w_buff->wp=%d w_buff->rp=%d \n",w_buff,w_buff->wp,w_buff->rp);
  put_reply();
//printf("w_buff=%p w_buff->wp=%d w_buff->rp=%d \n",w_buff,w_buff->wp,w_buff->rp);
    mark_sync();
}

//void bcm2835_spi_transfern(char* buf, uint32_t len);
/// Call bcm2835_spi_transfern with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_transfern(void)
{
uint32_t len;
char *buf;
    get_int_code();
    get_int_code();
    buf = *(char **)(buff+1);
    len = *(uint32_t *)(buff+5);
//  printf("B n buf=%p len=%d \n",buf,len);
    bcm2835_spi_transfern( bi_send_buff, len );
    set_ope_code( OPE_SPI_TRANSFERN );
    set_int_code( (int)buf );
    set_int_code( len );
    strncpy( bi_rec_buff, bi_send_buff, len );
//dump_buff();
//printf("w_buff=%p w_buff->wp=%d w_buff->rp=%d \n",w_buff,w_buff->wp,w_buff->rp);
  put_reply();
//printf("w_buff=%p w_buff->wp=%d w_buff->rp=%d \n",w_buff,w_buff->wp,w_buff->rp);
    mark_sync();
}

//void bcm2835_spi_writenb(char* buf, uint32_t len);
/// Call bcm2835_spi_writenb with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_spi_writenb(void)
{
uint32_t len;
    get_int_code();
    get_int_code();
    len = *(uint32_t *)(buff+5);
    bcm2835_spi_writenb( bi_send_buff, len );
}

//void bcm2835_i2c_begin(void);
/// Call bcm2835_i2c_begin
/// \par            Refer
/// \par            Modify
void ope_i2c_begin(void)
{
    bcm2835_i2c_begin();
}

//void bcm2835_i2c_end(void);
/// Call bcm2835_i2c_end
/// \par            Refer
/// \par            Modify
void ope_i2c_end(void)
{
    bcm2835_i2c_end();
}

//void bcm2835_i2c_setSlaveAddress(uint8_t addr);
/// Call bcm2835_i2c_setSlaveAddress with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_i2c_setslaveaddress(void)
{
    get_byte_code();
    bcm2835_i2c_setSlaveAddress( *((uint8_t *)(buff+1)) );
}

//void bcm2835_i2c_setClockDivider(uint16_t divider);
/// Call bcm2835_i2c_setClockDivider with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_i2c_setclockdivider(void)
{
    get_short_code();
    bcm2835_i2c_setClockDivider( *((uint16_t *)(buff+1)) );
}

//void bcm2835_i2c_set_baudrate(uint32_t baudrate);
/// Call bcm2835_i2c_set_baudrate with 1 parameter
/// \par            Refer
/// \par            Modify
void ope_i2c_set_baudrate(void)
{
    get_int_code();
    bcm2835_i2c_set_baudrate( *((uint32_t *)(buff+1)) );
}

//uint8_t bcm2835_i2c_write(const char * buf, uint32_t len);
/// Call bcm2835_i2c_write with 2 parameters
/// \par            Refer
///    - bi_send_buff
///    - buf *(uint32_t **)(buff+1)
///    - len *(uint32_t *)(buff+5)
/// \par            Modify
void ope_i2c_write(void)
{
uint8_t ret;
uint8_t *buf;
uint32_t len;

    get_int_code();
    buf = *(uint32_t **)(buff+1);
    get_int_code();
    len = *(uint32_t *)(buff+5);
    
    ret = bcm2835_i2c_write( bi_send_buff, *((uint32_t *)(buff+5))  );
    set_ope_code( OPE_I2C_WRITE );
    set_byte_code( ret );
    put_reply();
    mark_sync();
}

//uint8_t bcm2835_i2c_read(char* buf, uint32_t len);
/// Call bcm2835_i2c_read with 1 parameter
/// \par            Refer
///    - buf *(uint32_t **)(buff+1)
///    - len *(uint32_t *)(buff+5)
/// \par            Modify
///    - bi_receive_buff
void ope_i2c_read(void)
{
uint8_t ret;
char *buf;
uint32_t len;
    get_int_code();
    get_int_code();
    buf = *(char **)(buff+1);
    len = *(uint32_t *)(buff+5);  
    ret = bcm2835_i2c_read( bi_rec_buff, len );
    set_ope_code( OPE_I2C_READ );
    set_byte_code( ret );
    set_int_code( (int)buf );
    set_int_code( len );
    put_reply();
    mark_sync();
}

//uint8_t bcm2835_i2c_read_register_rs(char* regaddr, char* buf, uint32_t len);
/// Call bcm2835_i2c_read_register_rs with 3 parameter
/// \par            Refer
/// \par            Modify
void ope_i2c_read_register_rs(void)
{
uint8_t ret;
char* buf;
uint32_t len;
    get_byte_code();
    get_int_code();
    get_int_code();
    buf = *(char **)(buff+2);
    len = *(uint32_t *)(buff+6);
    ret = bcm2835_i2c_read_register_rs( (char *)(buff+1), bi_rec_buff, len );
    set_ope_code( OPE_I2C_READ_REGISTER_RS );
    set_byte_code( ret );
    set_int_code( (int)buf );
    set_int_code( len );
    put_reply();
}

//uint64_t bcm2835_st_read(void);
/// Call bcm2835_st_read
/// \par            Refer
/// \par            Modify
void ope_st_read(void)
{
uint64_t ret;
    ret = bcm2835_st_read();
    set_ope_code( OPE_ST_READ );
    set_long_code( ret );
    put_reply();
    mark_sync();
}

//void bcm2835_st_delay(uint64_t offset_micros, uint64_t micros);
/// Call bcm2835_st_delay with 2 parameters
/// \par            Refer
/// \par            Modify
void ope_st_delay(void)
{
    get_long_code();
    get_long_code();
    bcm2835_st_delay( *((uint64_t *)(buff+1)), *((uint64_t *)(buff+9)) );
}

//-------------------------
//----- SETUP USART 0 -----
//-------------------------
//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
int uart0_filestream = -1;  ///< file discripter for /dev/ttyAMA0

//OPEN THE UART
//The flags (defined in fcntl.h):
//	Access modes (use 1 of these):
//		O_RDONLY - Open for reading only.
//		O_RDWR - Open for reading and writing.
//		O_WRONLY - Open for writing only.
//
//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
//											immediately with a failure status if the output can't be written immediately.
//
//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.

/// Open uart 
/// \par            Refer
/// \par            Modify
/// \return         done:fd, err:-1
void open_uart(void)
{
  int flags;
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY );	//Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
       printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

// Is it need?
//  flags = fcntl(uart0_filestream, F_GETFL, 0);
//  fcntl (uart0_filestream, F_SETFL, flags | O_NONBLOCK );

  set_ope_code( OPE_OPEN_UART );
  set_int_code( uart0_filestream );
  put_reply();
  mark_sync();
}

//CONFIGURE THE UART
//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
//	CSIZE:- CS5, CS6, CS7, CS8
//	CLOCAL - Ignore modem status lines
//	CREAD - Enable receiver
//	IGNPAR = Ignore characters with parity errors
//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
//	PARENB - Parity enable
//	PARODD - Odd parity (else even)


/// Configure uart
/// \par            Refer
/// \par            Modify
void configure_uart( void )
{
struct termios options;
speed_t speed;
int baud;
  
  get_int_code();
  baud = *(int *)(buff+1);

  switch (baud)
  {
    case     50:	speed =     B50 ; break ;
    case     75:	speed =     B75 ; break ;
    case    110:	speed =    B110 ; break ;
    case    134:	speed =    B134 ; break ;
    case    150:	speed =    B150 ; break ;
    case    200:	speed =    B200 ; break ;
    case    300:	speed =    B300 ; break ;
    case    600:	speed =    B600 ; break ;
    case   1200:	speed =   B1200 ; break ;
    case   1800:	speed =   B1800 ; break ;
    case   2400:	speed =   B2400 ; break ;
    case   4800:	speed =   B4800 ; break ;
    case   9600:	speed =   B9600 ; break ;
    case  19200:	speed =  B19200 ; break ;
    case  38400:	speed =  B38400 ; break ;
    case  57600:	speed =  B57600 ; break ;
    case 115200:	speed = B115200 ; break ;
    case 230400:	speed = B230400 ; break ;
    default:
      speed =   B9600 ;
      printf("baudrate error %d \n",baud);
      break;
  }

  tcgetattr( uart0_filestream, &options );
  options.c_cflag = speed | CS8 | CLOCAL | CREAD;		//<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);
  put_reply();
  mark_sync();
}

//----- TX BYTES -----
/// \cond
unsigned char tx_buffer[20];
unsigned char *p_tx_buffer;
/// \endcond

/// Send uart
/// \par            Refer
///    - bi_send_buff
///    - buf *(uint32_t **)(buff+1)
///    - len *(uint32_t *)(buff+5)
/// \par            Modify
void send_uart( void )
{
  int count;
  char ret;
  uint8_t *buf;
  uint32_t len;

/*
  p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = 'H';
  *p_tx_buffer++ = 'e';
  *p_tx_buffer++ = 'l';
  *p_tx_buffer++ = 'l';
  *p_tx_buffer++ = 'o';
*/

  if (uart0_filestream == -1)
    {
      printf("UART is not opened \n");
      ret = -1;
    }
  else
    {
      get_int_code();
      buf = *(uint32_t **)(buff+1);
      get_int_code();
      len = *(uint32_t *)(buff+5);
      count = write(uart0_filestream, bi_send_buff, len);
      if (count < 0)
      {
          printf("UART TX error\n");
          ret = -1;
      }
      else{
        ret = 0;
      }
    }
    set_ope_code( OPE_SEND_UART );
    set_byte_code( ret );
    put_reply();
    mark_sync();
}

//----- CHECK FOR ANY RX BYTES -----
/// Receive uart
/// \par            Refer
///    - destination *(uint8_t **)(buff+1)
///    - max size *(uint32_t *)(buff+5)
/// \par            Modify
///    - bi_receive_buff
///    - ope_code *(uint8_t *)buff
///    - destination *(char **)(buff+1)
///    - receive size *(uint32_t *)(buff+5)
void receive_uart(void)
{
uint8_t *dest;
uint32_t max_len;
uint32_t rec_size;

  get_int_code();
  get_int_code();
  dest = *(uint8_t **)(buff+1);
  max_len = *(uint32_t *)(buff+5);  
  if (uart0_filestream != -1)
    {
      rec_size = read(uart0_filestream, bi_rec_buff, max_len);
      *(bi_rec_buff+rec_size) = '\0';
    }
  else
    {
      printf("UART is not opened \n");
      rec_size = -1;
    }
    set_ope_code( OPE_RECEIVE_UART );
    set_int_code( (uint32_t)dest );
    set_int_code( rec_size );
    put_reply();
    mark_sync();
}

//----- CLOSE THE UART -----
/// Close uart
/// \par            Refer
/// \par            Modify
///   - uart0_filestream
void close_uart(void)
{
  close(uart0_filestream);
  uart0_filestream = -1;
}

/// \cond
int main(int argc, char **argv)
{
    int fd;
    char *s;
    unsigned char ope_code;
    int len;
    int baud;
    char ret;
    
    printf("bcm2835_for_java ver1.00 start \n");
    if( argc != 2 )
    {
        printf("bcm2835_for_java needs 1 parameter like as /tmp/shared_mem\n");
      exit ( -1 );
    }
    else
    {
      fd = open( argv[1], O_RDWR);
      if( fd == -1 )
      {
        printf("file %s can't open\n",argv[1]);
        exit(-1);
      }
      else
      {
         s = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
         if( s == MAP_FAILED )
         {
            printf("we can't create mapping file\n");
         }
      }
    }

    r_buff = (struct ring_buff *)s;
    w_buff = (struct ring_buff *)(s+sizeof(struct ring_buff) );
    bi_send_buff = (char *)( s + 2*sizeof(struct ring_buff) );
    bi_rec_buff = (char *)( bi_send_buff + TEMP_BUFF_SIZE );
    sync_code = (int *)( bi_rec_buff + TEMP_BUFF_SIZE );
    reply_code = (int *)( sync_code + 1 );
    
    init_ring_buff(w_buff,WRITER_INIT);
    init_ring_buff(r_buff,READER_INIT);
    
    bi_status = STATUS_FINE;
    
    while( 1 )
    {
        if( calc_data_size( r_buff ) != 0 )
        {
            get_ope_code();
            ope_code = buff[0];
#ifdef DEBUG
          printf("bcm_for_java ope_code=0x%02x \n",ope_code);
          printf("r_buff->wp=%d r_buff->rp=%d \n",r_buff->wp,r_buff->rp);
#endif
          switch( ope_code )
            {
                case OPE_INIT:
                    bcm_init();
                    break;
                case OPE_CLOSE:
                    bcm_close();
                    break;
                case OPE_SET_DEBUG:
                    ope_set_debug();
                    break;
                case OPE_PERI_READ:
                    ope_peri_read();
                    break;
                case OPE_PERI_READ_NB:
                    ope_peri_read_nb();
                    break;
                case OPE_PERI_WRITE:
                    ope_peri_write();
                    break;
                case OPE_PERI_WRITE_NB:
                    ope_peri_write_nb();
                    break;
                case OPE_PERI_SET_BITS:
                    ope_peri_set_bits();
                    break;
                case OPE_GPIO_FSEL:
                    ope_gpio_fsel();
                    break;
                case OPE_GPIO_SET:
                    ope_gpio_set();
                    break;
                case OPE_GPIO_CLR:
                    ope_gpio_clr();
                    break;
                case OPE_GPIO_SET_MULTI:
                    ope_gpio_set_multi();
                    break;
                case OPE_GPIO_CLR_MULTI:
                    ope_gpio_clr_multi();
                    break;
                case OPE_GPIO_LEV:
                    ope_gpio_lev();
                    break;
                case OPE_GPIO_EDS:
                    ope_gpio_eds();
                    break;
                case OPE_GPIO_SET_EDS:
                    ope_gpio_set_eds();
                    break;
                case OPE_GPIO_REN:
                    ope_gpio_ren();
                    break;
                case OPE_GPIO_CLR_REN:
                    ope_gpio_clr_ren();
                    break;
                case OPE_GPIO_FEN:
                    ope_gpio_fen();
                    break;
                case OPE_GPIO_CLR_FEN:
                    ope_gpio_clr_fen();
                    break;
                case OPE_GPIO_HEN:
                    ope_gpio_hen();
                    break;
                case OPE_GPIO_CLR_HEN:
                    ope_gpio_clr_hen();
                    break;
                case OPE_GPIO_LEN:
                    ope_gpio_len();
                    break;
                case OPE_GPIO_CLR_LEN:
                    ope_gpio_clr_len();
                    break;
                case OPE_GPIO_AREN:
                    ope_gpio_aren();
                    break;
                case OPE_GPIO_CLR_AREN:
                    ope_gpio_clr_aren();
                    break;
                case OPE_GPIO_AFEN:
                    ope_gpio_afen();
                    break;
                case OPE_GPIO_CLR_AFEN:
                    ope_gpio_clr_afen();
                    break;
                case OPE_GPIO_PUD:
                    ope_gpio_pud();
                    break;
                case OPE_GPIO_PUDCLK:
                    ope_gpio_pudclk();
                    break;
                case OPE_GPIO_WRITE:
                    ope_gpio_write();
                    break;
                case OPE_GPIO_PAD:
                    ope_gpio_pad();
                    break;
                case OPE_GPIO_SET_PAD:
                    ope_gpio_set_pad();
                    break;
                case OPE_DELAY:
                    ope_delay();
                    break;
                case OPE_DELAYMICROSECONDS:
                    ope_delaymicroseconds();
                    break;
                case OPE_GPIO_WRITE_MULTI:
                    ope_gpio_write_multi();
                    break;
                case OPE_GPIO_WRITE_MASK:
                    ope_gpio_write_mask();
                    break;
                case OPE_GPIO_SET_PUD:
                    ope_gpio_set_pud();
                    break;
                case OPE_SPI_BEGIN:
                    ope_spi_begin();
                    break;
                case OPE_SPI_END:
                    ope_spi_end();
                    break;
                case OPE_SPI_SETBITORDER:
                    ope_spi_setbitorder();
                    break;
                case OPE_SPI_SETCLOCKDIVIDER:
                    ope_spi_setclockdivider();
                    break;
                case OPE_SPI_SETDATAMODE:
                    ope_spi_setdatamode();
                    break;
                case OPE_SPI_CHIPSELECT:
                    ope_spi_chipselect();
                    break;
                case OPE_SPI_SETCHIPSELECTPOLARITY:
                    ope_spi_setchipselectpolarity();
                    break;
                case OPE_SPI_TRANSFER:
                    ope_spi_transfer();
                    break;
                case OPE_SPI_TRANSFERNB:
                    ope_spi_transfernb();
                    break;
                case OPE_SPI_TRANSFERN:
                    ope_spi_transfern();
                    break;
                case OPE_SPI_WRITENB:
                    ope_spi_writenb();
                    break;
                case OPE_I2C_BEGIN:
                    ope_i2c_begin();
                    break;
                case OPE_I2C_END:
                    ope_i2c_end();
                    break;
                case OPE_I2C_SETSLAVEADDRESS:
                    ope_i2c_setslaveaddress();
                    break;
                case OPE_I2C_SETCLOCKDIVIDER:
                    ope_i2c_setclockdivider();
                    break;
                case OPE_I2C_SET_BAUDRATE:
                    ope_i2c_set_baudrate();
                    break;
                case OPE_I2C_WRITE:
                    ope_i2c_write();
                    break;
                case OPE_I2C_READ:
                    ope_i2c_read();
                    break;
                case OPE_I2C_READ_REGISTER_RS:
                    ope_i2c_read_register_rs();
                    break;
                case OPE_ST_READ:
                    ope_st_read();
                    break;
                case OPE_ST_DELAY:
                    ope_st_delay();
                    break;
                case OPE_HELLO:
                    get_int_code();
                    len = *(int *)(buff+1);
                    set_ope_code( OPE_REPLY );
                    strcpy( bi_rec_buff, "Nice to meet you." );
                    set_int_code( strlen(bi_rec_buff) );
                    put_reply();
                    mark_sync();
                    usleep(0);
                    break;
                case OPE_SYNC:
                    mark_sync();
                    break;
                case OPE_EXIT:
                    goto BREAK_LINE;
                    break;
                case OPE_OPEN_UART:
                    open_uart();
                    break;
                case OPE_CONFIG_UART:
                    configure_uart();
                    break;
                case OPE_SEND_UART:
                    send_uart();
                    break;
                case OPE_RECEIVE_UART:
                    receive_uart();
                    break;
                case OPE_CLOSE_UART:
                    close_uart();
                    break;
            }
        }
        else
        {
            usleep(0);
        }
    }
BREAK_LINE:
    printf("Close bcm2835_for_java\n");
    exit(0);
}
/// \endcond

/// \}
