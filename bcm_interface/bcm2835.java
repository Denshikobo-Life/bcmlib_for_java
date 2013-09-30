/******************************************************************************/
/*! @file       bcm2835.java
    @brief      This program is bcm2835 lapper library for java application.
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
    - 20XX/XX/XX anyone
      -# Add xxx 
      -# Delete yyy 
******************************************************************************/
package dkl;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

/// \defgroup bcm2835_java bcm2835_java
/// \ingroup PARENT
/// @{

/// Wapper class of bcm_interface ibrary
public class bcm2835 {
    
//public static final byte OPEN=0x1;
//public static final byte CLOSE=0x2;
    
/// This means pin HIGH, true, 3.3volts on a pin.
public static final byte HIGH=0x1;
/// This means pin LOW, false, 0volts on a pin.
public static final byte LOW=0x0;
/// Speed of the core clock core_clk
public static final int BCM2835_CORE_CLK_HZ=250000000;	///< 250 MHz

// Physical addresses for various peripheral register sets
/// Base Physical Address of the BCM 2835 peripheral registers
public static final int BCM2835_PERI_BASE=0x20000000;
/// Base Physical Address of the System Timer registers
public static final int BCM2835_ST_BASE=(BCM2835_PERI_BASE + 0x3000);
/// Base Physical Address of the Pads registers
public static final int BCM2835_GPIO_PADS=(BCM2835_PERI_BASE + 0x100000);
/// Base Physical Address of the Clock/timer registers
public static final int BCM2835_CLOCK_BASE=(BCM2835_PERI_BASE + 0x101000);
/// Base Physical Address of the GPIO registers
public static final int BCM2835_GPIO_BASE=(BCM2835_PERI_BASE + 0x200000);
/// Base Physical Address of the SPI0 registers
public static final int BCM2835_SPI0_BASE=(BCM2835_PERI_BASE + 0x204000);
/// Base Physical Address of the BSC0 registers
public static final int BCM2835_BSC0_BASE=(BCM2835_PERI_BASE + 0x205000);
/// Base Physical Address of the PWM registers
public static final int BCM2835_GPIO_PWM=(BCM2835_PERI_BASE + 0x20C000);
 /// Base Physical Address of the BSC1 registers
public static final int BCM2835_BSC1_BASE=(BCM2835_PERI_BASE + 0x804000);

/// Size of memory page on RPi
public static final int BCM2835_PAGE_SIZE=(4*1024);
/// Size of memory block on RPi
public static final int BCM2835_BLOCK_SIZE=(4*1024);

// Defines for GPIO
// The BCM2835 has 54 GPIO pins.
//      BCM2835 data sheet, Page 90 onwards.
/// GPIO register offsets from BCM2835_GPIO_BASE. Offsets into the GPIO Peripheral block in bytes per 6.1 Register View
public static final int BCM2835_GPFSEL0=0x0000; ///< GPIO Function Select 0
public static final int BCM2835_GPFSEL1=0x0004; ///< GPIO Function Select 1
public static final int BCM2835_GPFSEL2=0x0008; ///< GPIO Function Select 2
public static final int BCM2835_GPFSEL3=0x000c; ///< GPIO Function Select 3
public static final int BCM2835_GPFSEL4=0x0010; ///< GPIO Function Select 4
public static final int BCM2835_GPFSEL5=0x0014; ///< GPIO Function Select 5
public static final int BCM2835_GPSET0=0x001c; ///< GPIO Pin Output Set 0
public static final int BCM2835_GPSET1=0x0020; ///< GPIO Pin Output Set 1
public static final int BCM2835_GPCLR0=0x0028; ///< GPIO Pin Output Clear 0
public static final int BCM2835_GPCLR1=0x002c; ///< GPIO Pin Output Clear 1
public static final int BCM2835_GPLEV0=0x0034; ///< GPIO Pin Level 0
public static final int BCM2835_GPLEV1=0x0038; ///< GPIO Pin Level 1
public static final int BCM2835_GPEDS0=0x0040; ///< GPIO Pin Event Detect Status 0
public static final int BCM2835_GPEDS1=0x0044; ///< GPIO Pin Event Detect Status 1
public static final int BCM2835_GPREN0=0x004c; ///< GPIO Pin Rising Edge Detect Enable 0
public static final int BCM2835_GPREN1=0x0050; ///< GPIO Pin Rising Edge Detect Enable 1
public static final int BCM2835_GPFEN0=0x0058; ///< GPIO Pin Falling Edge Detect Enable 0
public static final int BCM2835_GPFEN1=0x005c; ///< GPIO Pin Falling Edge Detect Enable 1
public static final int BCM2835_GPHEN0=0x0064; ///< GPIO Pin High Detect Enable 0
public static final int BCM2835_GPHEN1=0x0068; ///< GPIO Pin High Detect Enable 1
public static final int BCM2835_GPLEN0=0x0070; ///< GPIO Pin Low Detect Enable 0
public static final int BCM2835_GPLEN1=0x0074; ///< GPIO Pin Low Detect Enable 1
public static final int BCM2835_GPAREN0=0x007c; ///< GPIO Pin Async. Rising Edge Detect 0
public static final int BCM2835_GPAREN1=0x0080; ///< GPIO Pin Async. Rising Edge Detect 1
public static final int BCM2835_GPAFEN0=0x0088; ///< GPIO Pin Async. Falling Edge Detect 0
public static final int BCM2835_GPAFEN1=0x008c; ///< GPIO Pin Async. Falling Edge Detect 1
public static final int BCM2835_GPPUD=0x0094; ///< GPIO Pin Pull-up/down Enable
public static final int BCM2835_GPPUDCLK0=0x0098; ///< GPIO Pin Pull-up/down Enable Clock 0
public static final int BCM2835_GPPUDCLK1=0x009c; ///< GPIO Pin Pull-up/down Enable Clock 1

public static final byte  BCM2835_GPIO_FSEL_INPT=0b000;   ///< Input
public static final byte    BCM2835_GPIO_FSEL_OUTP  = 0b001;   ///< Output
public static final byte    BCM2835_GPIO_FSEL_ALT0  = 0b100;   ///< Alternate function 0
public static final byte    BCM2835_GPIO_FSEL_ALT1  = 0b101;   ///< Alternate function 1
public static final byte    BCM2835_GPIO_FSEL_ALT2  = 0b110;   ///< Alternate function 2
public static final byte    BCM2835_GPIO_FSEL_ALT3  = 0b111;   ///< Alternate function 3
public static final byte    BCM2835_GPIO_FSEL_ALT4  = 0b011;   ///< Alternate function 4
public static final byte    BCM2835_GPIO_FSEL_ALT5  = 0b010;   ///< Alternate function 5
public static final byte    BCM2835_GPIO_FSEL_MASK  = 0b111;   ///< Function select bits mask

public static final byte    BCM2835_GPIO_PUD_OFF     = 0b00;   ///< Off ? disable pull-up/down
public static final byte    BCM2835_GPIO_PUD_DOWN    = 0b01;   ///< Enable Pull Down control
public static final byte    BCM2835_GPIO_PUD_UP      = 0b10;   ///< Enable Pull Up control

/// Pad control register offsets from BCM2835_GPIO_PADS
public static final int BCM2835_PADS_GPIO_0_27=0x002c; ///< Pad control register for pads 0 to 27
public static final int BCM2835_PADS_GPIO_28_45=0x0030; ///< Pad control register for pads 28 to 45
public static final int BCM2835_PADS_GPIO_46_53=0x0034; ///< Pad control register for pads 46 to 53

/// Pad Control masks
public static final int BCM2835_PAD_PASSWRD=(0x5A << 24);  ///< Password to enable setting pad mask
public static final int BCM2835_PAD_SLEW_RATE_UNLIMITED=0x10; ///< Slew rate unlimited
public static final int BCM2835_PAD_HYSTERESIS_ENABLED=0x08; ///< Hysteresis enabled
public static final int BCM2835_PAD_DRIVE_2mA=0x00; ///< 2mA drive current
public static final int BCM2835_PAD_DRIVE_4mA=0x01; ///< 4mA drive current
public static final int BCM2835_PAD_DRIVE_6mA=0x02; ///< 6mA drive current
public static final int BCM2835_PAD_DRIVE_8mA=0x03; ///< 8mA drive current
public static final int BCM2835_PAD_DRIVE_10mA=0x04; ///< 10mA drive current
public static final int BCM2835_PAD_DRIVE_12mA=0x05; ///< 12mA drive current
public static final int BCM2835_PAD_DRIVE_14mA=0x06; ///< 14mA drive current
public static final int BCM2835_PAD_DRIVE_16mA=0x07; ///< 16mA drive current

/// \brief bcm2835PadGroup
/// Pad group specification for bcm2835_gpio_pad()
public static final byte    BCM2835_PAD_GROUP_GPIO_0_27         = 0; ///< Pad group for GPIO pads 0 to 27
public static final byte    BCM2835_PAD_GROUP_GPIO_28_45        = 1; ///< Pad group for GPIO pads 28 to 45
public static final byte    BCM2835_PAD_GROUP_GPIO_46_53        = 2;  ///< Pad group for GPIO pads 46 to 53

/// \brief GPIO Pin Numbers
///
/// Here we define Raspberry Pin GPIO pins on P1 in terms of the underlying BCM GPIO pin numbers.
/// These can be passed as a pin number to any function requiring a pin.
/// Not all pins on the RPi 26 bin IDE plug are connected to GPIO pins
/// and some can adopt an alternate function.
/// RPi version 2 has some slightly different pinouts, and these are values RPI_V2_*.
/// At bootup, pins 8 and 10 are set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
/// When SPI0 is in use (ie after bcm2835_spi_begin()), pins 19, 21, 23, 24, 26 are dedicated to SPI
/// and cant be controlled independently
public static final byte    RPI_GPIO_P1_03        =  0;  ///< Version 1, Pin P1-03
public static final byte    RPI_GPIO_P1_05        =  1;  ///< Version 1, Pin P1-05
public static final byte    RPI_GPIO_P1_07        =  4;  ///< Version 1, Pin P1-07
public static final byte    RPI_GPIO_P1_08        = 14;  ///< Version 1, Pin P1-08, defaults to alt function 0 UART0_TXD
public static final byte    RPI_GPIO_P1_10        = 15;  ///< Version 1, Pin P1-10, defaults to alt function 0 UART0_RXD
public static final byte    RPI_GPIO_P1_11        = 17;  ///< Version 1, Pin P1-11
public static final byte    RPI_GPIO_P1_12        = 18;  ///< Version 1, Pin P1-12
public static final byte    RPI_GPIO_P1_13        = 21;  ///< Version 1, Pin P1-13
public static final byte    RPI_GPIO_P1_15        = 22;  ///< Version 1, Pin P1-15
public static final byte    RPI_GPIO_P1_16        = 23;  ///< Version 1, Pin P1-16
public static final byte    RPI_GPIO_P1_18        = 24;  ///< Version 1, Pin P1-18
public static final byte    RPI_GPIO_P1_19        = 10;  ///< Version 1, Pin P1-19, MOSI when SPI0 in use
public static final byte    RPI_GPIO_P1_21        =  9;  ///< Version 1, Pin P1-21, MISO when SPI0 in use
public static final byte    RPI_GPIO_P1_22        = 25;  ///< Version 1, Pin P1-22
public static final byte    RPI_GPIO_P1_23        = 11;  ///< Version 1, Pin P1-23, CLK when SPI0 in use
public static final byte    RPI_GPIO_P1_24        =  8;  ///< Version 1, Pin P1-24, CE0 when SPI0 in use
public static final byte    RPI_GPIO_P1_26        =  7;  ///< Version 1, Pin P1-26, CE1 when SPI0 in use

    // RPi Version 2
public static final byte    RPI_V2_GPIO_P1_03     =  2;  ///< Version 2, Pin P1-03
public static final byte    RPI_V2_GPIO_P1_05     =  3;  ///< Version 2, Pin P1-05
public static final byte    RPI_V2_GPIO_P1_07     =  4;  ///< Version 2, Pin P1-07
public static final byte    RPI_V2_GPIO_P1_08     = 14;  ///< Version 2, Pin P1-08, defaults to alt function 0 UART0_TXD
public static final byte    RPI_V2_GPIO_P1_10     = 15;  ///< Version 2, Pin P1-10, defaults to alt function 0 UART0_RXD
public static final byte    RPI_V2_GPIO_P1_11     = 17;  ///< Version 2, Pin P1-11
public static final byte    RPI_V2_GPIO_P1_12     = 18;  ///< Version 2, Pin P1-12
public static final byte    RPI_V2_GPIO_P1_13     = 27;  ///< Version 2, Pin P1-13
public static final byte    RPI_V2_GPIO_P1_15     = 22;  ///< Version 2, Pin P1-15
public static final byte    RPI_V2_GPIO_P1_16     = 23;  ///< Version 2, Pin P1-16
public static final byte    RPI_V2_GPIO_P1_18     = 24;  ///< Version 2, Pin P1-18
public static final byte    RPI_V2_GPIO_P1_19     = 10;  ///< Version 2, Pin P1-19, MOSI when SPI0 in use
public static final byte    RPI_V2_GPIO_P1_21     =  9;  ///< Version 2, Pin P1-21, MISO when SPI0 in use
public static final byte    RPI_V2_GPIO_P1_22     = 25;  ///< Version 2, Pin P1-22
public static final byte    RPI_V2_GPIO_P1_23     = 11;  ///< Version 2, Pin P1-23, CLK when SPI0 in use
public static final byte    RPI_V2_GPIO_P1_24     =  8;  ///< Version 2, Pin P1-24, CE0 when SPI0 in use
public static final byte    RPI_V2_GPIO_P1_26     =  7;  ///< Version 2, Pin P1-26, CE1 when SPI0 in use

    // RPi Version 2, new plug P5
public static final byte    RPI_V2_GPIO_P5_03     = 28;  ///< Version 2, Pin P5-03
public static final byte    RPI_V2_GPIO_P5_04     = 29;  ///< Version 2, Pin P5-04
public static final byte    RPI_V2_GPIO_P5_05     = 30;  ///< Version 2, Pin P5-05
public static final byte    RPI_V2_GPIO_P5_06     = 31;  ///< Version 2, Pin P5-06

// Defines for SPI
// GPIO register offsets from BCM2835_SPI0_BASE. 
// Offsets into the SPI Peripheral block in bytes per 10.5 SPI Register Map
public static final int BCM2835_SPI0_CS=0x0000; ///< SPI Master Control and Status
public static final int BCM2835_SPI0_FIFO=0x0004; ///< SPI Master TX and RX FIFOs
public static final int BCM2835_SPI0_CLK=0x0008; ///< SPI Master Clock Divider
public static final int BCM2835_SPI0_DLEN=0x000c; ///< SPI Master Data Length
public static final int BCM2835_SPI0_LTOH=0x0010; ///< SPI LOSSI mode TOH
public static final int BCM2835_SPI0_DC=0x0014; ///< SPI DMA DREQ Controls

// Register masks for SPI0_CS
public static final int BCM2835_SPI0_CS_LEN_LONG=0x02000000; ///< Enable Long data word in Lossi mode if DMA_LEN is set
public static final int BCM2835_SPI0_CS_DMA_LEN=0x01000000; ///< Enable DMA mode in Lossi mode
public static final int BCM2835_SPI0_CS_CSPOL2=0x00800000; ///< Chip Select 2 Polarity
public static final int BCM2835_SPI0_CS_CSPOL1=0x00400000; ///< Chip Select 1 Polarity
public static final int BCM2835_SPI0_CS_CSPOL0=0x00200000; ///< Chip Select 0 Polarity
public static final int BCM2835_SPI0_CS_RXF=0x00100000; ///< RXF - RX FIFO Full
public static final int BCM2835_SPI0_CS_RXR=0x00080000; ///< RXR RX FIFO needs Reading ( full)
public static final int BCM2835_SPI0_CS_TXD=0x00040000; ///< TXD TX FIFO can accept Data
public static final int BCM2835_SPI0_CS_RXD=0x00020000; ///< RXD RX FIFO contains Data
public static final int BCM2835_SPI0_CS_DONE=0x00010000; ///< Done transfer Done
public static final int BCM2835_SPI0_CS_TE_EN=0x00008000; ///< Unused
public static final int BCM2835_SPI0_CS_LMONO=0x00004000; ///< Unused
public static final int BCM2835_SPI0_CS_LEN=0x00002000; ///< LEN LoSSI enable
public static final int BCM2835_SPI0_CS_REN=0x00001000; ///< REN Read Enable
public static final int BCM2835_SPI0_CS_ADCS=0x00000800; ///< ADCS Automatically Deassert Chip Select
public static final int BCM2835_SPI0_CS_INTR=0x00000400; ///< INTR Interrupt on RXR
public static final int BCM2835_SPI0_CS_INTD=0x00000200; ///< INTD Interrupt on Done
public static final int BCM2835_SPI0_CS_DMAEN=0x00000100; ///< DMAEN DMA Enable
public static final int BCM2835_SPI0_CS_TA=0x00000080; ///< Transfer Active
public static final int BCM2835_SPI0_CS_CSPOL=0x00000040; ///< Chip Select Polarity
public static final int BCM2835_SPI0_CS_CLEAR=0x00000030; ///< Clear FIFO Clear RX and TX
public static final int BCM2835_SPI0_CS_CLEAR_RX=0x00000020; ///< Clear FIFO Clear RX 
public static final int BCM2835_SPI0_CS_CLEAR_TX=0x00000010; ///< Clear FIFO Clear TX 
public static final int BCM2835_SPI0_CS_CPOL=0x00000008; ///< Clock Polarity
public static final int BCM2835_SPI0_CS_CPHA=0x00000004; ///< Clock Phase
public static final int BCM2835_SPI0_CS_CS=0x00000003; ///< Chip Select

/// \brief bcm2835SPIBitOrder SPI Bit order
/// Specifies the SPI data bit ordering for bcm2835_spi_setBitOrder()
public static final byte    BCM2835_SPI_BIT_ORDER_LSBFIRST = 0;  ///< LSB First
public static final byte    BCM2835_SPI_BIT_ORDER_MSBFIRST = 1;   ///< MSB First

/// \brief SPI Data mode
/// Specify the SPI data mode to be passed to bcm2835_spi_setDataMode()
public static final byte    BCM2835_SPI_MODE0 = 0;  ///< CPOL = 0, CPHA = 0
public static final byte    BCM2835_SPI_MODE1 = 1;  ///< CPOL = 0, CPHA = 1
public static final byte    BCM2835_SPI_MODE2 = 2;  ///< CPOL = 1, CPHA = 0
public static final byte    BCM2835_SPI_MODE3 = 3;  ///< CPOL = 1, CPHA = 1

/// \brief bcm2835SPIChipSelect
/// Specify the SPI chip select pin(s)
public static final byte    BCM2835_SPI_CS0 = 0;     ///< Chip Select 0
public static final byte    BCM2835_SPI_CS1 = 1;     ///< Chip Select 1
public static final byte    BCM2835_SPI_CS2 = 2;     ///< Chip Select 2 (ie pins CS1 and CS2 are asserted)
public static final byte    BCM2835_SPI_CS_NONE = 3; ///< No CS, control it yourself

/// \brief bcm2835SPIClockDivider
/// Specifies the divider used to generate the SPI clock from the system clock.
/// Figures below give the divider, clock period and clock frequency.
/// Clock divided is based on nominal base clock rate of 250MHz
/// It is reported that (contrary to the documentation) any even divider may used.
/// The frequencies shown for each divider have been confirmed by measurement
public static final int    BCM2835_SPI_CLOCK_DIVIDER_65536 = 0;       ///< 65536 = 262.144us = 3.814697260kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_32768 = 32768;   ///< 32768 = 131.072us = 7.629394531kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_16384 = 16384;   ///< 16384 = 65.536us = 15.25878906kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192;   ///< 8192 = 32.768us = 30/51757813kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_4096  = 4096;    ///< 4096 = 16.384us = 61.03515625kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_2048  = 2048;    ///< 2048 = 8.192us = 122.0703125kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_1024  = 1024;    ///< 1024 = 4.096us = 244.140625kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_512   = 512;     ///< 512 = 2.048us = 488.28125kHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_256   = 256;     ///< 256 = 1.024us = 976.5625MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_128   = 128;     ///< 128 = 512ns = = 1.953125MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_64    = 64;      ///< 64 = 256ns = 3.90625MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_32    = 32;      ///< 32 = 128ns = 7.8125MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_16    = 16;      ///< 16 = 64ns = 15.625MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_8     = 8;       ///< 8 = 32ns = 31.25MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_4     = 4;       ///< 4 = 16ns = 62.5MHz
public static final int    BCM2835_SPI_CLOCK_DIVIDER_2     = 2;       ///< 2 = 8ns = 125MHz, fastest you can get
public static final int    BCM2835_SPI_CLOCK_DIVIDER_1     = 1;       ///< 0 = 262.144us = 3.814697260kHz, same as 0/65536

// Defines for I2C
// GPIO register offsets from BCM2835_BSC*_BASE.
// Offsets into the BSC Peripheral block in bytes per 3.1 BSC Register Map
public static final int BCM2835_BSC_C=0x0000; ///< BSC Master Control
public static final int BCM2835_BSC_S=0x0004; ///< BSC Master Status
public static final int BCM2835_BSC_DLEN=0x0008; ///< BSC Master Data Length
public static final int BCM2835_BSC_A=0x000c; ///< BSC Master Slave Address
public static final int BCM2835_BSC_FIFO=0x0010; ///< BSC Master Data FIFO
public static final int BCM2835_BSC_DIV=0x0014; ///< BSC Master Clock Divider
public static final int BCM2835_BSC_DEL=0x0018; ///< BSC Master Data Delay
public static final int BCM2835_BSC_CLKT=0x001c; ///< BSC Master Clock Stretch Timeout

// Register masks for BSC_C
public static final int BCM2835_BSC_C_I2CEN=0x00008000; ///< I2C Enable, 0 = disabled, 1 = enabled
public static final int BCM2835_BSC_C_INTR=0x00000400; ///< Interrupt on RX
public static final int BCM2835_BSC_C_INTT=0x00000200; ///< Interrupt on TX
public static final int BCM2835_BSC_C_INTD=0x00000100; ///< Interrupt on DONE
public static final int BCM2835_BSC_C_ST=0x00000080; ///< Start transfer, 1 = Start a new transfer
public static final int BCM2835_BSC_C_CLEAR_1=0x00000020; ///< Clear FIFO Clear
public static final int BCM2835_BSC_C_CLEAR_2=0x00000010; ///< Clear FIFO Clear
public static final int BCM2835_BSC_C_READ=0x00000001; ///<	Read transfer

// Register masks for BSC_S
public static final int BCM2835_BSC_S_CLKT=0x00000200; ///< Clock stretch timeout
public static final int BCM2835_BSC_S_ERR=0x00000100; ///< ACK error
public static final int BCM2835_BSC_S_RXF=0x00000080; ///< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
public static final int BCM2835_BSC_S_TXE=0x00000040; ///< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
public static final int BCM2835_BSC_S_RXD=0x00000020; ///< RXD FIFO contains data
public static final int BCM2835_BSC_S_TXD=0x00000010; ///< TXD FIFO can accept data
public static final int BCM2835_BSC_S_RXR=0x00000008; ///< RXR FIFO needs reading (full)
public static final int BCM2835_BSC_S_TXW=0x00000004; ///< TXW FIFO needs writing (full)
public static final int BCM2835_BSC_S_DONE=0x00000002; ///< Transfer DONE
public static final int BCM2835_BSC_S_TA=0x00000001; ///< Transfer Active

public static final int BCM2835_BSC_FIFO_SIZE=16; ///< BSC FIFO size

/// \brief bcm2835I2CClockDivider
/// Specifies the divider used to generate the I2C clock from the system clock.
/// Clock divided is based on nominal base clock rate of 250MHz
public static final int     BCM2835_I2C_CLOCK_DIVIDER_2500   = 2500;      ///< 2500 = 10us = 100 kHz
public static final int     BCM2835_I2C_CLOCK_DIVIDER_626    = 626;       ///< 622 = 2.504us = 399.3610 kHz
public static final int     BCM2835_I2C_CLOCK_DIVIDER_150    = 150;       ///< 150 = 60ns = 1.666 MHz (default at reset)
public static final int     BCM2835_I2C_CLOCK_DIVIDER_148    = 148;       ///< 148 = 59ns = 1.689 MHz

/// \brief bcm2835I2CReasonCodes
/// Specifies the reason codes for the bcm2835_i2c_write and bcm2835_i2c_read functions.
public static final byte  BCM2835_I2C_REASON_OK   		 = 0x00;      ///< Success
public static final byte     BCM2835_I2C_REASON_ERROR_NACK    = 0x01;      ///< Received a NACK
public static final byte     BCM2835_I2C_REASON_ERROR_CLKT    = 0x02;      ///< Received Clock Stretch Timeout
public static final byte     BCM2835_I2C_REASON_ERROR_DATA    = 0x04;      ///< Not all data is sent / received

// Defines for ST
// GPIO register offsets from BCM2835_ST_BASE.
// Offsets into the ST Peripheral block in bytes per 12.1 System Timer Registers
// The System Timer peripheral provides four 32-bit timer channels and a single 64-bit free running counter.
// BCM2835_ST_CLO is the System Timer Counter Lower bits register.
// The system timer free-running counter lower register is a read-only register that returns the current value
// of the lower 32-bits of the free running counter.
// BCM2835_ST_CHI is the System Timer Counter Upper bits register.
// The system timer free-running counter upper register is a read-only register that returns the current value
// of the upper 32-bits of the free running counter.
public static final int BCM2835_ST_CS =	0x0000; ///< System Timer Control/Status
public static final int BCM2835_ST_CLO =0x0004; ///< System Timer Counter Lower 32 bits
public static final int BCM2835_ST_CHI =0x0008; ///< System Timer Counter Upper 32 bits

/// @}


// Defines for PWM
public static final int BCM2835_PWM_CONTROL= 0; ///< Address offset of PWM_CONTROL
public static final int BCM2835_PWM_STATUS = 1; ///< Address offset of PWM_STATUS
public static final int BCM2835_PWM0_RANGE = 4; ///< Address offset of PWM0_RANGE
public static final int BCM2835_PWM0_DATA  = 5; ///< Address offset of PWM0_DATA
public static final int BCM2835_PWM1_RANGE = 8; ///< Address offset of BCM2835_PWM1_RANGE
public static final int BCM2835_PWM1_DATA  = 9; ///< Address offset of BCM2835_PWM1_DATA 

public static final int BCM2835_PWMCLK_CNTL =    40; ///< Address offset of ???
public static final int BCM2835_PWMCLK_DIV  =    41; ///< Address offset of ???

public static final int BCM2835_PWM1_MS_MODE =   0x8000;  ///< Run in MS mode
public static final int BCM2835_PWM1_USEFIFO =   0x2000;  ///< Data from FIFO
public static final int BCM2835_PWM1_REVPOLAR=   0x1000;  ///< Reverse polarity
public static final int BCM2835_PWM1_OFFSTATE=   0x0800;  ///< Ouput Off state
public static final int BCM2835_PWM1_REPEATFF =  0x0400;  ///< Repeat last value if FIFO empty
public static final int BCM2835_PWM1_SERIAL  =   0x0200;  ///< Run in serial mode
public static final int BCM2835_PWM1_ENABLE   =  0x0100;  ///< Channel Enable

public static final int BCM2835_PWM0_MS_MODE =   0x0080;  ///< Run in MS mode
public static final int BCM2835_PWM0_USEFIFO =   0x0020;  ///< Data from FIFO
public static final int BCM2835_PWM0_REVPOLAR =  0x0010;  ///< Reverse polarity
public static final int BCM2835_PWM0_OFFSTATE =  0x0008;  ///< Ouput Off state
public static final int BCM2835_PWM0_REPEATFF =  0x0004;  ///< Repeat last value if FIFO empty
public static final int BCM2835_PWM0_SERIAL   =  0x0002;  ///< Run in serial mode
public static final int BCM2835_PWM0_ENABLE   =  0x0001;  ///< Channel Enable

/// Wapper of bi_init and bcm2835_init 
public static native int bi_init( String args );
/// Wapper of bi_close and bcm2835_close 
public static native int bi_close();
/// Wapper of bcm2835_set_debug 
public static native void set_debug(byte debug);
/// Wapper of bcm2835_peri_read 
public static native int peri_read(int paddr);
/// Wapper of bcm2835_peri_read_nb 
public static native int peri_read_nb(int paddr);
/// Wapper of bcm2835_peri_write 
public static native void peri_write(int paddr, int value);
/// Wapper of bcm2835_peri_write_nb 
public static native void peri_write_nb(int paddr, int value);
/// Wapper of bcm2835_peri_set_bits 
public static native void peri_set_bits(int paddr, int value, int mask);
/// Wapper of bcm2835_gpio_fsel 
public static native void gpio_fsel(byte pin, byte mode);
/// Wapper of bcm2835_gpio_set 
public static native void gpio_set(byte pin);
/// Wapper of bcm2835_gpio_clr 
public static native void gpio_clr(byte pin);
/// Wapper of bcm2835_gpio_set_multi 
public static native void gpio_set_multi(int mask);
/// Wapper of bcm2835_gpio_clr_multi 
public static native void gpio_clr_multi(int mask);
/// Wapper of bcm2835_gpio_lev 
public static native byte gpio_lev(byte pin);
/// Wapper of bcm2835_gpio_eds 
public static native byte gpio_eds(byte pin);
/// Wapper of bcm2835_gpio_set_eds 
    public static native void gpio_set_eds(byte pin);
/// Wapper of bcm2835_gpio_ren 
    public static native void gpio_ren(byte pin);
/// Wapper of bcm2835_gpio_clr_ren 
    public static native void gpio_clr_ren(byte pin);
/// Wapper of bcm2835_gpio_fen 
    public static native void gpio_fen(byte pin);
/// Wapper of bcm2835_gpio_clr_fen 
    public static native void gpio_clr_fen(byte pin);
/// Wapper of bcm2835_gpio_hen 
    public static native void gpio_hen(byte pin);
/// Wapper of bcm2835_gpio_clr_hen 
    public static native void gpio_clr_hen(byte pin);
/// Wapper of bcm2835_gpio_len 
    public static native void gpio_len(byte pin);
/// Wapper of bcm2835_gpio_clr_len 
    public static native void gpio_clr_len(byte pin);
/// Wapper of bcm2835_gpio_aren 
    public static native void gpio_aren(byte pin);
/// Wapper of bcm2835_gpio_clr_aren 
    public static native void gpio_clr_aren(byte pin);
/// Wapper of bcm2835_gpio_afen 
    public static native void gpio_afen(byte pin);
/// Wapper of bcm2835_gpio_clr_afen 
    public static native void gpio_clr_afen(byte pin);
/// Wapper of bcm2835_gpio_pud 
    public static native void gpio_pud(byte pud);
/// Wapper of bcm2835_gpio_pudclk 
    public static native void gpio_pudclk(byte pin, byte on);
/// Wapper of bcm2835_gpio_pad 
    public static native  int gpio_pad(byte group);
/// Wapper of bcm2835_gpio_set_pad 
    public static native void gpio_set_pad(byte group, int control);
/// Wapper of bcm2835_delay 
    public static native void bi_delay (int millis);
/// Wapper of bcm2835_delayMicroseconds 
    public static native void bi_delayMicroseconds (long micros);
/// Wapper of bcm2835_gpio_write 
    public static native void gpio_write(byte pin, byte on);
/// Wapper of bcm2835_gpio_write_multi 
    public static native void gpio_write_multi(int mask, byte on);
/// Wapper of bcm2835_gpio_write_mask 
    public static native void gpio_write_mask(int value, int mask);
/// Wapper of bcm2835_gpio_set_pud 
    public static native void gpio_set_pud(byte pin, byte pud);
/// Wapper of bcm2835_spi_begin 
    public static native void spi_begin();
/// Wapper of bcm2835_spi_end 
    public static native void spi_end();
/// Wapper of bcm2835_spi_setBitOrder 
    public static native void spi_setBitOrder(byte order);
/// Wapper of bcm2835_spi_setClockDivider 
    public static native void spi_setClockDivider(short divider);
/// Wapper of bcm2835_spi_setDataMode 
    public static native void spi_setDataMode(byte mode);
/// Wapper of bcm2835_spi_chipSelect 
    public static native void spi_chipSelect(byte cs);
/// Wapper of bcm2835_spi_setChipSelectPolarity 
    public static native void spi_setChipSelectPolarity(byte cs, byte active);
/// Wapper of bcm2835_spi_transfer 
    public static native byte spi_transfer(byte value);
/// Wapper of bcm2835_spi_transfernb 
    public static native void spi_transfernb(String tbuf, String rbuf, int len);
/// Wapper of bcm2835_spi_transfern 
    public static native void spi_transfern(String buf, int len);
/// Wapper of bcm2835_spi_writenb 
    public static native void spi_writenb(String buf, int len);
/// Wapper of bcm2835_i2c_begin 
    public static native void i2c_begin();
/// Wapper of bcm2835_i2c_end 
    public static native void i2c_end();
/// Wapper of bcm2835_i2c_setSlaveAddress 
    public static native void i2c_setSlaveAddress(byte addr);
/// Wapper of bcm2835_i2c_setClockDivider 
    public static native void i2c_setClockDivider(short divider);
/// Wapper of bcm2835_i2c_set_baudrate 
    public static native void i2c_set_baudrate(int baudrate);
/// Wapper of bcm2835_i2c_write 
    public static native byte i2c_write(String buf, int len);
/// Wapper of bcm2835_i2c_read 
    public static native byte i2c_read( byte[] buf, int len);
/// Wapper of bcm2835_i2c_read_register_rs 
    public static native byte i2c_read_register_rs(String regaddr, String buf, int len);
/// Wapper of bcm2835_st_read 
    public static native long st_read();
/// Wapper of bcm2835_st_delay 
    public static native void st_delay(long offset_micros, long micros);
/// Wapper of ope_sync 
    public static native  int ope_sync();
    
    static {
        Native.register("bcm_interface");
    }
}
/// @}
