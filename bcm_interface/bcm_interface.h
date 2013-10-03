/********************************************************************
 FileName:      bcm_interface.h
 Dependencies:  See INCLUDES section
 Processor:	ARM11
 Hardware:      Paspberry pi
 Complier:  	gcc
 Company:	Denchikobo-Life,Ltd.

 Software License Agreement:

 The software supplied herewith by Denchikobo-Life,Ltd.
 is intended and supplied to you, the out customer, for use solely and
 exclusively on our products. The software is owned by Denchikobo-Life,Ltd.
 And is protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. Denchikobo-Life,Ltd. SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release
********************************************************************/
// Defines for BCM2835
#ifndef BCM_INTERFACE_H
#define BCM_INTERFACE_H

#include <stdint.h>

#include "../common/bi_common.h"
#include "bcm2835.h"

#ifdef __cplusplus
extern "C" {
#endif

    int ope_init(void);
    int ope_close(void);
    void set_debug(uint8_t debug);
    uint32_t peri_read(volatile uint32_t* paddr);
    uint32_t peri_read_nb(volatile uint32_t* paddr);
    void peri_write(volatile uint32_t* paddr, uint32_t value);
    void peri_write_nb(volatile uint32_t* paddr, uint32_t value);
    void peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask);
    void gpio_fsel(uint8_t pin, uint8_t mode);
    void gpio_set(uint8_t pin);
    void gpio_clr(uint8_t pin);
    void gpio_set_multi(uint32_t mask);
    void gpio_clr_multi(uint32_t mask);
    uint8_t gpio_lev(uint8_t pin);
    uint8_t gpio_eds(uint8_t pin);
    void gpio_set_eds(uint8_t pin);
    void gpio_ren(uint8_t pin);
    void gpio_clr_ren(uint8_t pin);
    void gpio_fen(uint8_t pin);
    void gpio_clr_fen(uint8_t pin);
    void gpio_hen(uint8_t pin);
    void gpio_clr_hen(uint8_t pin);
    void gpio_len(uint8_t pin);
    void gpio_clr_len(uint8_t pin);
    void gpio_aren(uint8_t pin);
    void gpio_clr_aren(uint8_t pin);
    void gpio_afen(uint8_t pin);
    void gpio_clr_afen(uint8_t pin);
    void gpio_pud(uint8_t pud);
    void gpio_pudclk(uint8_t pin, uint8_t on);
    uint32_t gpio_pad(uint8_t group);
    void gpio_set_pad(uint8_t group, uint32_t control);
    void bi_delay (unsigned int millis);
    void bi_delayMicroseconds (uint64_t micros);
    void gpio_write(uint8_t pin, uint8_t on);
    void gpio_write_multi(uint32_t mask, uint8_t on);
    void gpio_write_mask(uint32_t value, uint32_t mask);
    void gpio_set_pud(uint8_t pin, uint8_t pud);
    void spi_begin(void);
    void spi_end(void);
    void spi_setBitOrder(uint8_t order);
    void spi_setClockDivider(uint16_t divider);
    void spi_setDataMode(uint8_t mode);
    void spi_chipSelect(uint8_t cs);
    void spi_setChipSelectPolarity(uint8_t cs, uint8_t active);
    uint8_t spi_transfer(uint8_t value);
    void spi_transfernb(char* tbuf, char* rbuf, uint32_t len);
    void spi_transfern(char* buf, uint32_t len);
    void spi_writenb(char* buf, uint32_t len);
    void i2c_begin(void);
    void i2c_end(void);
    void i2c_setSlaveAddress(uint8_t addr);
    void i2c_setClockDivider(uint16_t divider);
    void i2c_set_baudrate(uint32_t baudrate);
    uint8_t i2c_write(const char * buf, uint32_t len);
    uint8_t i2c_read(char* buf, uint32_t len);
    uint8_t i2c_read_register_rs(char* regaddr, char* buf, uint32_t len);
    uint64_t st_read(void);
    void st_delay(uint64_t offset_micros, uint64_t micros);
    int ope_hello( const char *str);
    int ope_exit( void );
    int ope_open_uart( void );
    void ope_config_uart( int baud );
    char ope_send_uart( const char * buf, uint32_t len );
    int ope_receive_uart( char* buf, uint32_t len );
    void ope_close_uart( void );

#ifdef __cplusplus
}
#endif



#endif // BCM2835_H
