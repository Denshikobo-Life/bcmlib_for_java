/******************************************************************************/
/*! @file       bi_bcm2835.c
    @brief      This program is Interface between Java app and shared ring_buff
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
#include "bcm_interface.h"

/// \defgroup bi_bcm2835 bi_bcm2835
/// \ingroup PARENT
/// @{

//
// Low level register access functions
//

//uint8_t uint8_t_ret;
//int int_ret;
//uint64_t uint64_t_ret;

/// Copy send data from local buff to shared ring buff
/// \param[in] rb shared ring buffer ( Java to bcm )
/// \param[in] str local buffer
/// \param[in] len byte size 
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
int put_command(struct ring_buff *rb, char *str, int len )
{
    while( put_ring_buff( w_buff, buff, wp ) != 0 )
    {
        if( context_switch(100) != 0 )return -1;
    }
    return 0;
}

//extern int local_sync_code;   ///< base sync code(counter)

/// Set sync_code, and clear reply_code.
/// \par            Refer
/// \par            Modify
void set_sync( void )
{
//    local_sync_code++;
//    *sync_code = local_sync_code;
//    *reply_code = 0;
    *reply_code = *sync_code;
    *sync_code += 1;
}

/// Check sync have done, or break on time_out condition.
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
int check_sync( void )
{
int loop_count;
#ifdef DEBUG
    printf("sync_code=0x%04x\n",*sync_code);
#endif
    for(loop_count=0;loop_count < 10000 ;loop_count++)
    {
        if( *reply_code == *sync_code )
        {
#ifdef DEBUG
          printf("reply_code=0x%04x\n",*reply_code);
#endif
            return 0;
        }
        usleep(1000);
    }
    printf("Error sync_code=0x%04x\n",*sync_code);
    printf("Error reply_code=0x%04x\n",*reply_code);
    return -1;
}

/// Send command and return result of execution(type int)
/// \param[in] rb  ring_buff ( Java to bcm )
/// \param[in] str local buffer
/// \param[in] len byte size 
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
int send_command_1(struct ring_buff *rb, char *str, int len)
{
  int check_return;
  set_sync();
#ifdef DEBUG
  printf("command=0x%02x \n",*buff);
#endif
  if ( put_command( rb, str, len ) != 0 ) return -1;
  check_return = check_sync();
#ifdef DEBUG
  printf("checksync=%d \n",check_return);
#endif
  get_ope_code();
  get_int_code();
  return *(int *)(buff+1); 
}

/// Send command and copy result into des(type byte[])
/// \param[in] rb  ring_buff ( Java to bcm )
/// \param[in] str local buffer
/// \param[in] len byte size 
/// \par            Refer
/// \par            Modify
/// \return         done:0, time_over:-1
int send_command_2(struct ring_buff *rb, char *str, int len)
{
char *dest;
uint32_t dlen;
  if ( put_command( rb, str, len ) != 0 ) return -1;
  if( context_switch(100) != 0 ) return -1;
  get_ope_code();
  get_int_code();
  get_int_code();
  dest = *(char **)(buff+1);
  dlen = *(uint32_t *)(buff+5);
  copy_str( dest, bi_rec_buff, dlen);
  return 0;
}

/// Send command, copy result into dest(type byte[]) and return result of execution(type uint8_t)
/// \param[in] rb  ring_buff ( Java to bcm )
/// \param[in] str local buffer
/// \param[in] len byte size 
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
uint8_t send_command_3(struct ring_buff *rb, char *str, int len)
{
char *dest;
uint32_t dlen;
  if ( put_command( rb, str, len ) != 0 ) return -1;
  if( context_switch(100) != 0 ) return -1;
  get_ope_code();
  get_byte_code();
  get_int_code();
  get_int_code();
  dest = *(char **)(buff+2);
  dlen = *(uint32_t *)(buff+6);
  copy_str( dest, bi_rec_buff, dlen);
  return *(uint8_t *)(buff+1); 
}

//  void  bcm2835_set_debug(uint8_t debug);
/// Send bcm_2835_set_debug command
/// \param[in] d 0:debug off 1:debug on
/// \par            Refer
/// \par            Modify
void set_debug(uint8_t d)
{
   set_ope_code( OPE_SET_DEBUG );
   set_byte_code( d );
   (void)put_command(  w_buff, buff, wp );
}

//  int bcm2835_init(void);
/// Send bcm2835_init command and return result of execution(type int)
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
int ope_init(void)
{
   set_ope_code( OPE_INIT );
   return send_command_1( w_buff, buff, wp );
}

//  int bcm2835_close(void);
/// Send bcm2835_close command and return result of execution(type int)
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
int ope_close(void)
{
   set_ope_code( OPE_CLOSE );
   return send_command_1( w_buff, buff, wp );
}

//  uint32_t bcm2835_peri_read(volatile uint32_t* paddr);
/// Send bcm2835_peri_read command and return result of execution(type uint32_t)
/// \param[in] paddr physical address of target
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
uint32_t peri_read(volatile uint32_t* paddr)
{
   set_ope_code( OPE_PERI_READ );
   set_int_code( (int)paddr );
   return (uint32_t)send_command_1( w_buff, buff, wp );
}

//  uint32_t bcm2835_peri_read_nb(volatile uint32_t* paddr);
/// Send bcm2835_peri_reead _nb command and return result of execution(type uint32_t)
/// \param[in] paddr physical address of target
/// \par            Refer
/// \par            Modify
/// \return         done:result , time_over:-1
uint32_t peri_read_nb(volatile uint32_t* paddr)
{
   set_ope_code( OPE_PERI_READ_NB );
   set_int_code( (int)paddr );
   return (uint32_t)send_command_1( w_buff, buff, wp );
}

//  void bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value);
/// Send bcm2835_peri_write command and 2 parameter
/// \param[in] paddr physical address of target
/// \param[in] value write data on target
/// \par            Refer
/// \par            Modify
void peri_write(volatile uint32_t* paddr, uint32_t value)
{
   set_ope_code( OPE_PERI_WRITE );
   set_int_code( (int)paddr );
   set_int_code( (int)value );
   (void)put_command(  w_buff, buff, wp );
}

//  void bcm2835_peri_write_nb(volatile uint32_t* paddr, uint32_t value);
/// Send bcm2835_peri_write_nb command and 2 parameter
/// \param[in] paddr physical address of target
/// \param[in] value write data on target
/// \par            Refer
/// \par            Modify
void peri_write_nb(volatile uint32_t* paddr, uint32_t value)
{
   set_ope_code( OPE_PERI_WRITE_NB );
   set_int_code( (int)paddr );
   set_int_code( (int)value );
   (void)put_command( w_buff, buff, wp );
}

// Set/clear only the bits in value covered by the mask

//  void bcm2835_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask);
/// Send bcm2835_peri_set_bits command and 3 parameter
/// \param[in] paddr physical address of target
/// \param[in] value write data on target
/// \param[in] mask write mask data
/// \par            Refer
/// \par            Modify
void peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask)
{
   set_ope_code( OPE_PERI_SET_BITS );
   set_int_code( (int)paddr );
   set_int_code( (int)value );
   set_int_code( (int)mask );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode);
/// Send bcm2835_peri_fsel command and 2 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \param[in] mode write data on the pin
/// \par            Refer
/// \par            Modify
void gpio_fsel(uint8_t pin, uint8_t mode)
{
   set_ope_code( OPE_GPIO_FSEL );
   set_byte_code( pin );
   set_byte_code( mode );
   (void)put_command( w_buff, buff, wp );
}

// Set output pin
//  void bcm2835_gpio_set(uint8_t pin);
/// Send bcm2835_gpio_set command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_set(uint8_t pin)
{
   set_ope_code( OPE_GPIO_SET );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Clear output pin
//  void bcm2835_gpio_clr(uint8_t pin);
/// Send bcm2835_gpio_clr command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Set all output pins in the mask
//  void bcm2835_gpio_set_multi(uint32_t mask);
/// Send bcm2835_gpio_set_multi command and 1 parameter
/// \param[in] mask uint32_t
/// \par            Refer
/// \par            Modify
void gpio_set_multi(uint32_t mask)
{
   set_ope_code( OPE_GPIO_SET_MULTI );
   set_int_code( mask );
   (void)put_command( w_buff, buff, wp );
}

// Clear all output pins in the mask
//  void bcm2835_gpio_clr_multi(uint32_t mask);
/// Send bcm2835_gpio_clr_multi command and 1 parameter
/// \param[in] mask uint32_t
/// \par            Refer
/// \par            Modify
void gpio_clr_multi(uint32_t mask)
{
   set_ope_code( OPE_GPIO_CLR_MULTI );
   set_int_code( mask );
   (void)put_command( w_buff, buff, wp );
}

// Read input pin
//  uint8_t bcm2835_gpio_lev(uint8_t pin);
/// Send bcm2835_gpio_lev command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
uint8_t gpio_lev(uint8_t pin)
{
   set_ope_code( OPE_GPIO_LEV );
   set_byte_code( pin );
   return (uint8_t)send_command_1( w_buff, buff, wp );
}

// See if an event detection bit is set
// Sigh cant support interrupts yet
//  uint8_t bcm2835_gpio_eds(uint8_t pin);
/// Send bcm2835_gpio_eds command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
uint8_t gpio_eds(uint8_t pin)
{
   set_ope_code( OPE_GPIO_EDS );
   set_byte_code( pin );
   return send_command_1( w_buff, buff, wp );
}

// Write a 1 to clear the bit in EDS
//  void bcm2835_gpio_set_eds(uint8_t pin);
/// Send bcm2835_gpio_set_eds command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_set_eds(uint8_t pin)
{
   set_ope_code( OPE_GPIO_SET_EDS );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Rising edge detect enable
//  void bcm2835_gpio_ren(uint8_t pin);
/// Send bcm2835_gpio_ren command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_ren(uint8_t pin)
{
   set_ope_code( OPE_GPIO_REN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_ren(uint8_t pin);
/// Send bcm2835_gpio_clr_ren command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_ren(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR_REN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Falling edge detect enable
//  void bcm2835_gpio_fen(uint8_t pin);
/// Send bcm2835_gpio_fen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_fen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_FEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_fen(uint8_t pin);
/// Send bcm2835_gpio_clr_fen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_fen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR_FEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// High detect enable
//  void bcm2835_gpio_hen(uint8_t pin);
/// Send bcm2835_gpio_hen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_hen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_HEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_hen(uint8_t pin);
/// Send bcm2835_gpio_clr_hen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_hen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR_HEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Low detect enable
//  void bcm2835_gpio_len(uint8_t pin);
/// Send bcm2835_gpio_len command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_len(uint8_t pin)
{
   set_ope_code( OPE_GPIO_LEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_len(uint8_t pin);
/// Send bcm2835_gpio_clt_len command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_len(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR_LEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Async rising edge detect enable
//  void bcm2835_gpio_aren(uint8_t pin);
/// Send bcm2835_gpio_aren command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_aren(uint8_t pin)
{
   set_ope_code( OPE_GPIO_AREN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_aren(uint8_t pin);
/// Send bcm2835_gpio_clt_aren command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_aren(uint8_t pin)
{
   set_ope_code( OPE_GPIO_AREN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Async falling edge detect enable
//  void bcm2835_gpio_afen(uint8_t pin);
/// Send bcm2835_gpio_afen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_afen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_AFEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_clr_afen(uint8_t pin);
/// Send bcm2835_gpio_clr_afen command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \par            Refer
/// \par            Modify
void gpio_clr_afen(uint8_t pin)
{
   set_ope_code( OPE_GPIO_CLR_AFEN );
   set_byte_code( pin );
   (void)put_command( w_buff, buff, wp );
}

// Set pullup/down
//  void bcm2835_gpio_pud(uint8_t pud);
/// Send bcm2835_gpio_pud command and 1 parameter
/// \param[in] pud uint8_t
/// \par            Refer
/// \par            Modify
void gpio_pud(uint8_t pud)
{
   set_ope_code( OPE_GPIO_PUD );
   set_byte_code( pud );
   (void)put_command( w_buff, buff, wp );
}

// Pullup/down clock
// Clocks the value of pud into the GPIO pin
//  void bcm2835_gpio_pudclk(uint8_t pin, uint8_t on);
/// Send bcm2835_gpio_pudclk command and 1 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \param[in] on  pullup confition 0:down  1:up
/// \par            Refer
/// \par            Modify
void gpio_pudclk(uint8_t pin, uint8_t on)
{
   set_ope_code( OPE_GPIO_PUDCLK );
   set_byte_code( pin );
   set_byte_code( on );
   (void)put_command( w_buff, buff, wp );
}

// Read GPIO pad behaviour for groups of GPIOs
//  uint32_t bcm2835_gpio_pad(uint8_t group);
/// Send bcm2835_gpio_pad command and 1 parameter
/// \param[in] group group number like as "BCM2835_PAD_GROUP_GPIO_0_27"
/// \par            Refer
/// \par            Modify
uint32_t gpio_pad(uint8_t group)
{
   set_ope_code( OPE_GPIO_PAD );
   set_byte_code( group );
   return (uint32_t)send_command_1( w_buff, buff, wp );
}

// Set GPIO pad behaviour for groups of GPIOs
// powerup value for al pads is
// BCM2835_PAD_SLEW_RATE_UNLIMITED | BCM2835_PAD_HYSTERESIS_ENABLED | BCM2835_PAD_DRIVE_8mA

//  void bcm2835_gpio_set_pad(uint8_t group, uint32_t control);
/// Send bcm2835_gpio_set_pad command and 2 parameter
/// \param[in] group group number like as "BCM2835_PAD_GROUP_GPIO_0_27"
/// \param[in] control uint32_t 
/// \par            Refer
/// \par            Modify
void gpio_set_pad(uint8_t group, uint32_t control)
{
   set_ope_code( OPE_GPIO_SET_PAD );
   set_byte_code( group );
   set_int_code( control );
   (void)put_command( w_buff, buff, wp );
}

// Some convenient arduino-like functions
// milliseconds
//  void bcm2835_delay (unsigned int millis);
/// Send bcm2835_delay command and 1 parameter
/// \param[in] millis unsigned int 
/// \par            Refer
/// \par            Modify
void bi_delay(unsigned int millis)
{
   set_ope_code( OPE_DELAY );
   set_int_code( millis );
   (void)put_command( w_buff, buff, wp );
}

// microseconds
//  void bcm2835_delayMicroseconds (uint64_t micros);
//  void bcm2835_delay (unsigned int millis);
/// Send bcm2835_delayMicroseconds command and 1 parameter
/// \param[in] micros uint64_t 
/// \par            Refer
/// \par            Modify
void bi_delayMicroseconds(uint64_t micros)
{
   set_ope_code( OPE_DELAYMICROSECONDS );
   set_long_code( micros );
   (void)put_command( w_buff, buff, wp );
}

//
// Higher level convenience functions
//

// Set the state of an output
//  void bcm2835_gpio_write(uint8_t pin, uint8_t on);
/// Send bcm2835_gpio_write command and 2 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \param[in] on uint8_t 
/// \par            Refer
/// \par            Modify
void gpio_write(uint8_t pin, uint8_t on)
{
   set_ope_code( OPE_GPIO_WRITE );
   set_byte_code( pin );
   set_byte_code( on );
   (void)put_command( w_buff, buff, wp );
}

// Set the state of a all 32 outputs in the mask to on or off
//  void bcm2835_gpio_write_multi(uint32_t mask, uint8_t on);
/// Send bcm2835_gpio_write_multi command and 2 parameter
/// \param[in] mask uint32_t 
/// \param[in] on uint8_t 
/// \par            Refer
/// \par            Modify
void gpio_write_multi(uint32_t mask, uint8_t on)
{
   set_ope_code( OPE_GPIO_WRITE_MULTI );
   set_int_code( mask );
   set_byte_code( on );
   (void)put_command( w_buff, buff, wp );
}

// Set the state of a all 32 outputs in the mask to the values in value
//  void bcm2835_gpio_write_mask(uint32_t value, uint32_t mask);
/// Send bcm2835_gpio_write_mask command and 2 parameter
/// \param[in] value uint32_t 
/// \param[in] mask uint32_t 
/// \par            Refer
/// \par            Modify
void gpio_write_mask(uint32_t value, uint32_t mask)
{
   set_ope_code( OPE_GPIO_WRITE_MASK );
   set_int_code( value );
   set_int_code( mask );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_gpio_set_pud(uint8_t pin, uint8_t pud);
/// Send bcm2835_gpio_set_pad command and 2 parameter
/// \param[in] pin pin number like as "RPI_GPIO_P1_03"
/// \param[in] pud uint8_t 
/// \par            Refer
/// \par            Modify
void gpio_set_pud(uint8_t pin, uint8_t pud)
{
   set_ope_code( OPE_GPIO_SET_PUD );
   set_byte_code( pin );
   set_byte_code( pud );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_begin(void);
/// Send bcm2835_spi_degin command
/// \par            Refer
/// \par            Modify
void spi_begin(void)
{
   set_ope_code( OPE_SPI_BEGIN );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_end(void);
/// Send bcm2835_spi_end command
/// \par            Refer
/// \par            Modify
void spi_end(void)
{  
   set_ope_code( OPE_SPI_END );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_setBitOrder(uint8_t order);
/// Send bcm2835_spi_setBitOrder command and 1 parameter
/// \param[in] order uint8_t 
/// \par            Refer
/// \par            Modify
void spi_setBitOrder(uint8_t order)
{
    // BCM2835_SPI_BIT_ORDER_MSBFIRST is the only one suported by SPI0
   set_ope_code( OPE_SPI_SETBITORDER );
   set_byte_code( order );
   (void)put_command( w_buff, buff, wp );
}

// defaults to 0, which means a divider of 65536.
// The divisor must be a power of 2. Odd numbers
// rounded down. The maximum SPI clock rate is
// of the APB clock
//  void bcm2835_spi_setClockDivider(uint16_t divider);
/// Send bcm2835_spi_setClockDivider command and 1 parameter
/// \param[in] divider uint16_t 
/// \par            Refer
/// \par            Modify
void spi_setClockDivider(uint16_t divider)
{
   set_ope_code( OPE_SPI_SETCLOCKDIVIDER );
   set_short_code( divider );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_setDataMode(uint8_t mode);
/// Send bcm2835_spi_setDataMode command and 1 parameter
/// \param[in] mode uint8_t 
/// \par            Refer
/// \par            Modify
void spi_setDataMode(uint8_t mode)
{
   set_ope_code( OPE_SPI_SETDATAMODE );
   set_byte_code( mode );
   (void)put_command( w_buff, buff, wp );
}

// Writes (and reads) a single byte to SPI
//  uint8_t bcm2835_spi_transfer(uint8_t value);
/// Send bcm2835_spi_transfer command and 1 parameter
/// \param[in] value uint8_t 
/// \par            Refer
/// \par            Modify
uint8_t spi_transfer(uint8_t value)
{
   set_ope_code( OPE_SPI_TRANSFER );
   set_byte_code( value );
   return (uint8_t)put_command( w_buff, buff, wp );
}

// Writes (and reads) an number of bytes to SPI
//  void bcm2835_spi_transfernb(char* tbuf, char* rbuf, uint32_t len);
/// Send bcm2835_spi_transfernb command and 3 parameter
/// \param[in] tbuf char* 
/// \param[out] rbuf char* 
/// \param[in]  len uint32_t 
/// \par            Refer
/// \par            Modify
void spi_transfernb(char* tbuf, char* rbuf, uint32_t len)
{
   set_ope_code( OPE_SPI_TRANSFERNB );
   set_int_code( (int)tbuf );
   set_int_code( (int)rbuf );
   set_int_code( len );
   copy_str( bi_send_buff, tbuf, len );
   (void)send_command_3( w_buff, buff, wp );
}

// Writes (and reads) an number of bytes to SPI
//  void bcm2835_spi_transfern(char* buf, uint32_t len);
/// Send bcm2835_spi_transfern command and 2 parameter
/// \param tbuf char* 
/// \param[in] len uint32_t 
/// \par            Refer
/// \par            Modify
void spi_transfern(char* tbuf, uint32_t len)
{
   set_ope_code( OPE_SPI_TRANSFERN );
   set_int_code( (int)tbuf );
   set_int_code( len );
   copy_str( bi_send_buff, tbuf, len );
   (void)send_command_3( w_buff, buff, wp );
}

// Writes (and reads) an number of bytes to SPI
// Read bytes are copied over onto the transmit buffer
//  void bcm2835_spi_writenb(char* buf, uint32_t len);
/// Send bcm2835_spi_writenb command and 2 parameter
/// \param[in] buf char* 
/// \param[in] len uint32_t 
/// \par            Refer
/// \par            Modify
void spi_writenb(char* buf, uint32_t len)
{
   set_ope_code( OPE_SPI_WRITENB );
   set_int_code( (int)buf );
   set_int_code( len );
   copy_str( bi_send_buff, buf, len );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_chipSelect(uint8_t cs);
/// Send bcm2835_spi_chipSelect command and 1 parameter
/// \param[in] cs uint8_t 
/// \par            Refer
/// \par            Modify
void spi_chipSelect(uint8_t cs)
{
   set_ope_code( OPE_SPI_CHIPSELECT );
   set_byte_code( cs );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_spi_setChipSelectPolarity(uint8_t cs, uint8_t active);
/// Send bcm2835_spi_setChipSelectPolarity command and 2 parameter
/// \param[in] cs uint8_t 
/// \param[in] active uint8_t 
/// \par            Refer
/// \par            Modify
void spi_setChipSelectPolarity(uint8_t cs, uint8_t active)
{
   set_ope_code( OPE_SPI_SETCHIPSELECTPOLARITY );
   set_byte_code( cs );
   set_byte_code( active );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_i2c_begin(void);
/// Send bcm2835_i2c_begin command
/// \par            Refer
/// \par            Modify
void i2c_begin(void)
{
   set_ope_code( OPE_I2C_BEGIN );
   (void)put_command( w_buff, buff, wp );
}

//  void bcm2835_i2c_end(void);
/// Send bcm2835_i2c_end command
/// \par            Refer
/// \par            Modify
void i2c_end(void)
{
   set_ope_code( OPE_I2C_END );
   (void)put_command( w_buff, buff, wp );
}


//  void bcm2835_i2c_setSlaveAddress(uint8_t addr);
/// Send bcm2835_i2c_setSlaveAddress command and 1 parameter
/// \param[in] addr uint8_t 
/// \par            Refer
/// \par            Modify
void i2c_setSlaveAddress(uint8_t addr)
{
   set_ope_code( OPE_I2C_SETSLAVEADDRESS );
   set_byte_code( addr );
   (void)put_command( w_buff, buff, wp );
}

// defaults to 0x5dc, should result in a 166.666 kHz I2C clock frequency.
// The divisor must be a power of 2. Odd numbers
// rounded down.
//  void bcm2835_i2c_setClockDivider(uint16_t divider);
/// Send bcm2835_i2c_setClockDivider command and 1 parameter
/// \param[in] divider uint16_t 
/// \par            Refer
/// \par            Modify
void i2c_setClockDivider(uint16_t divider)
{
   set_ope_code( OPE_I2C_SETCLOCKDIVIDER );
   set_short_code( divider );
   (void)put_command( w_buff, buff, wp );
}

// set I2C clock divider by means of a baudrate number
//  void bcm2835_i2c_set_baudrate(uint32_t baudrate);
/// Send bcm2835_i2c_set_baudrate command and 1 parameter
/// \param[in] baudrate uint32_t 
/// \par            Refer
/// \par            Modify
void i2c_set_baudrate(uint32_t baudrate)
{
   set_ope_code( OPE_I2C_SET_BAUDRATE );
   set_int_code( baudrate );
   (void)put_command( w_buff, buff, wp );
}

// Writes an number of bytes to I2C
//  uint8_t bcm2835_i2c_write(const char * buf, uint32_t len);
/// Send bcm2835_i2c_write command and 2 parameter, and return result of execution(type uint8_t)
/// \param[in] buf const char * 
/// \param[in] len uint32_t 
/// \par            Refer
/// \par            Modify
/// \return         done:result , executin error:-1
uint8_t i2c_write(const char * buf, uint32_t len)
{
int ret;
   set_ope_code( OPE_I2C_WRITE );
   set_int_code( (int)buf );
   set_int_code( len );
   copy_str( bi_send_buff, (char *)buf, len );
   set_sync();
   ret = put_command( w_buff, buff, wp );
   if ( ret != 0 ) return (uint8_t)(-1);
   ret = check_sync();
   if( ret == 0 )
   {
     get_ope_code();
     get_byte_code();
     return *(uint8_t *)(buff+1); 
   }
   else
   {
       return (uint8_t)(-1);
   }
}

// Read an number of bytes from I2C
//  uint8_t bcm2835_i2c_read(char* buf, uint32_t len);
/// Send bcm2835_i2c_read command and 2 parameter, and return result of execution(type uint8_t)
/// \param[out] buf char * 
/// \param[out] len uint32_t 
/// \par            Refer
/// \par            Modify
/// \return         done:result , executin error:-1
uint8_t i2c_read(char* buf, uint32_t len)
{
int ret;
int dlen;
char *dest;
   set_ope_code( OPE_I2C_READ );
   set_int_code( (int)buf );
   set_int_code( len );

   set_sync();
   ret = put_command( w_buff, buff, wp );
   if ( ret != 0 ) return (uint8_t)(-1);
   ret = check_sync();
   if ( ret != 0 ) return (uint8_t)(-1);
   get_ope_code();
   get_byte_code();
   get_int_code();
   get_int_code();
   dest = *(char **)(buff+2);
   dlen = *(uint32_t *)(buff+6);
   if( (int)buf == (int)dest )
   {
     copy_str( dest, bi_rec_buff, dlen);
     return *(uint8_t *)(buff+1); 
   }
   else
   {
      return (uint8_t)(-1);
   }
}

// Read an number of bytes from I2C sending a repeated start after writing
// the required register. Only works if your device supports this mode
//  uint8_t bcm2835_i2c_read_register_rs(char* regaddr, char* buf, uint32_t len);
/// Send bcm2835_i2c_read_register_rs command and 3 parameter, and return result of execution(type uint8_t)
/// \param[in] regaddr char * 
/// \param[out] buf char * 
/// \param[in] len uint32_t 
/// \par            Refer
/// \par            Modify
/// \return         done:result , executin error:-1
uint8_t i2c_read_register_rs(char* regaddr, char* buf, uint32_t len)
{   
   set_ope_code( OPE_I2C_READ_REGISTER_RS );
   set_byte_code( *regaddr );   // <== !!!!
   set_int_code( (int)buf );
   set_int_code( len );
   set_sync();

  if ( put_command( w_buff, buff, wp ) != 0 ) return (uint8_t)(-1);
  if( context_switch(100) != 0 ) return (uint8_t)(-1);
  get_ope_code();
  get_int_code();
  return *(uint8_t *)(buff+1); 
}

// Read the System Timer Counter (64-bits)
//  uint64_t bcm2835_st_read(void);
/// Send bcm2835_st_read command, and return result of execution(type uint64_t)
/// \par            Refer
/// \par            Modify
/// \return         done:result , executin error:-1
uint64_t st_read(void)
{
   set_ope_code( OPE_ST_READ );
   set_sync();
   if ( put_command( w_buff, buff, wp ) != 0 ) return (uint64_t)(-1);
   if( context_switch(100) != 0 ) return (uint64_t)(-1);
   get_ope_code();
   get_long_code();
   return *(uint64_t *)(buff+1);
}

// Delays for the specified number of microseconds with offset
//  void bcm2835_st_delay(uint64_t offset_micros, uint64_t micros);
/// Send bcm2835_st_delay command and 2 parameter
/// \param[in] offset_micros uint64_t 
/// \param[in] micros uint64_t 
/// \par            Refer
/// \par            Modify
/// \return         done:result , executin error:-1
void st_delay(uint64_t offset_micros, uint64_t micros)
{
   set_ope_code( OPE_ST_DELAY );
   set_long_code( offset_micros );
   set_long_code( micros );
   (void)put_command( w_buff, buff, wp );
}

/// \cond 
int ope_hello( const char *str )
{
int len;
int ret;
   len = strlen(str);
   set_ope_code( OPE_HELLO );
   set_int_code( len );
   copy_str( bi_send_buff, (char *)str, len );
   set_sync();
   ret = put_command( w_buff, buff, wp );
   if ( ret != 0 )return -1;
   ret = check_sync();
   
   if( ret == 0 )
   {
      get_ope_code();
      get_int_code();
      len = *(int *)(buff+1);
//      *(bi_rec_buff+len) = '\0';
//      printf("Reply %s \n", bi_rec_buff);
      return 0;
   }
   else
   {
      return -1;
   }
}
/// \endcond 

/// Send ope_sync command, and return result of sync(type int)
/// \par            Refer
/// \par            Modify
/// \return         done: 0 , time out:-1
int ope_sync( void )
{
    set_sync();
    set_ope_code( OPE_SYNC );
    if ( put_command( w_buff, buff, wp ) != 0 )return -1;
    return check_sync();
}

/// Send ope_exit command, and return result of executin(type int)
/// \par            Refer
/// \par            Modify
/// \return         done: 0 , time out:-1
int ope_exit( void )
{
int ret;    
   set_ope_code( OPE_EXIT );
   ret = put_command( w_buff, buff, wp );
   if ( ret == 0 ) ret = context_switch(10000);
   return ret;
}

/// Send ope_open_uart command, and return result of executin(type int)
/// \par            Refer
/// \par            Modify
/// \return         done: file discripter , error:-1
int ope_open_uart( void )
{
   set_ope_code( OPE_OPEN_UART );
   return send_command_1( w_buff, buff, wp );
}


/// Send ope_config_uart command, and return result of executin(type int)
/// \param[in] baud int
/// \par            Refer
/// \par            Modify
/// \return         done: 0 , time out:-1
void ope_config_uart( int baud )
{
   set_ope_code( OPE_CONFIG_UART );
   set_int_code( baud );
   (void)send_command_1( w_buff, buff, wp );
}


/// Send ope_send_uart command, and return result of executin(type int)
/// \param[in]      buf const char *
/// \param[in]      len uint32_t 
/// \par            Refer
/// \par            Modify
/// \return         done: 0 , time out:-1
char ope_send_uart( const char * buf, uint32_t len )
{
   set_ope_code( OPE_SEND_UART );
   set_int_code( (int)buf );
   set_int_code( len );
   copy_str( bi_send_buff, (char *)buf, len );
   return (char)send_command_1( w_buff, buff, wp );
}

/// Call ope_receive_uart command, and return result of executin(type int)
/// \param[out] buf char * 
/// \param[out] len uint32_t 
/// \par            Refer
/// \par            Refer
/// \par            Modify
/// \return         receive size or 0
int ope_receive_uart(char* buf, uint32_t len)
{
int ret;
int dlen;
char *dest;
  
   set_ope_code( OPE_RECEIVE_UART );
   set_int_code( (int)buf );
   set_int_code( len );

   set_sync();
   ret = put_command( w_buff, buff, wp );
   if ( ret != 0 ) return (uint8_t)(-1);
   ret = check_sync();
   if ( ret != 0 ) return (uint8_t)(-1);
   get_ope_code();
   get_int_code();  // dest
   get_int_code();  // size
   dest = *(char **)(buff+1);
   dlen = *(uint32_t *)(buff+5);
   if( (int)buf == (int)dest )
   {
     copy_str( dest, bi_rec_buff, dlen);
     return dlen; 
   }
   else
   {
      return 0;
   }
}

/// Send ope_close_uart command, and return result of executin(type int)
/// \par            Refer
/// \par            Modify
/// \return         done: 0 , time out:-1
void ope_close_uart( void )
{
  set_ope_code( OPE_CLOSE_UART );
  (void)put_command( w_buff, buff, wp );
}

/// @}
