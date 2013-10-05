/********************************************************************
 FileName:      bi_commoon.h
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
2013/09/18  1.0   Initial release
2013/09/30  1.1   corrent typo
********************************************************************/
#ifndef BI_COMMON_H
#define	BI_COMMON_H

//#define DEBUG

#ifdef	__cplusplus
extern "C" {
#endif


extern struct ring_buff *r_buff,*w_buff;    
extern char *bi_send_buff;
extern char *bi_rec_buff;
extern char buff[];
extern int wp;
extern union int_char u_ic;
extern union long_char u_lc;
extern int bi_status;
extern volatile int *sync_code;
extern volatile int *reply_code;

#define FLAG_ON 1
#define FLAG_OFF 0

/// convert between int to char[]
union int_char
{    
    int i;             ///< i=0x12345678
    char c[4];         ///<  c[0]=78 c[1]=56 c[2]=34 c[3]=12
    short int s[2];    ///< s[0]=0x5678 s[1]=0x1234 
    int *p;            ///< p=0x12345678
};

/// convert between long int to char[]
union long_char
{
    long l;             ///< 0x12345678 22334455
    int i[2];           ///< i[0]=0x22334455 i[1]=0x12345678
    char c[8];          ///< c[0]=55 c[1]=44 c[2]=33 c[3]=22
};
    
#define OPE_INIT 0x30
#define OPE_CLOSE 0x31
#define OPE_SET_DEBUG 0x32
#define OPE_PERI_READ 0x33
#define OPE_PERI_READ_NB 0x34
#define OPE_PERI_WRITE 0x35
#define OPE_PERI_WRITE_NB 0x36
#define OPE_PERI_SET_BITS 0x37
#define OPE_GPIO_FSEL 0x38
#define OPE_GPIO_SET 0x39
#define OPE_GPIO_CLR 0x3a
#define OPE_GPIO_SET_MULTI 0x3b
#define OPE_GPIO_CLR_MULTI 0x3c
#define OPE_GPIO_LEV 0x3d
#define OPE_GPIO_EDS 0x3e
#define OPE_GPIO_SET_EDS 0x3f
#define OPE_GPIO_REN 0x40
#define OPE_GPIO_CLR_REN 0x41
#define OPE_GPIO_FEN 0x42
#define OPE_GPIO_CLR_FEN 0x43
#define OPE_GPIO_HEN 0x44
#define OPE_GPIO_CLR_HEN 0x45
#define OPE_GPIO_LEN 0x46
#define OPE_GPIO_CLR_LEN 0x47
#define OPE_GPIO_AREN 0x48
#define OPE_GPIO_CLR_AREN 0x49
#define OPE_GPIO_AFEN 0x4a
#define OPE_GPIO_CLR_AFEN 0x4b
#define OPE_GPIO_PUD 0x4c
#define OPE_GPIO_PUDCLK 0x4d

#define OPE_GPIO_PAD 0x4f
#define OPE_GPIO_SET_PAD 0x50
#define OPE_DELAY 0x51
#define OPE_DELAYMICROSECONDS 0x52
#define OPE_GPIO_WRITE 0x4e
#define OPE_GPIO_WRITE_MULTI 0x53
#define OPE_GPIO_WRITE_MASK 0x54
#define OPE_GPIO_SET_PUD 0x55
#define OPE_SPI_BEGIN 0x56
#define OPE_SPI_END 0x57
#define OPE_SPI_SETBITORDER 0x58
#define OPE_SPI_SETCLOCKDIVIDER 0x59
#define OPE_SPI_SETDATAMODE 0x5a
#define OPE_SPI_CHIPSELECT 0x5b
#define OPE_SPI_SETCHIPSELECTPOLARITY 0x5c
#define OPE_SPI_TRANSFER 0x5d
#define OPE_SPI_TRANSFERNB 0x5e
#define OPE_SPI_TRANSFERN 0x5f

#define OPE_SPI_WRITENB 0x61
#define OPE_I2C_BEGIN 0x62
#define OPE_I2C_END 0x63
#define OPE_I2C_SETSLAVEADDRESS 0x64
#define OPE_I2C_SETCLOCKDIVIDER 0x65
#define OPE_I2C_SET_BAUDRATE 0x66
#define OPE_I2C_WRITE 0x67
#define OPE_I2C_READ 0x68
#define OPE_I2C_READ_REGISTER_RS 0x69
#define OPE_ST_READ 0x6a
#define OPE_ST_DELAY 0x6b
#define OPE_HELLO 0x6c
#define OPE_REPLY 0x6d
#define OPE_SYNC 0x6e
#define OPE_EXIT 0x70

#define OPE_OPEN_UART 0x71
#define OPE_CONFIG_UART 0x72
#define OPE_SEND_UART 0x73
#define OPE_RECEIVE_UART 0x74
#define OPE_CLOSE_UART 0x75

// Define new function codes
//#define OPE_***** 0x76
//#define OPE_***** 0x77
//#define OPE_***** 0x78

// context_switch_mode
#define JAVA_TO_BCM 1
#define BCM_TO_JAVA 2

// bi_status
#define STATUS_FINE 0
#define COUNT_OVER -10
#define OPE_CODE_UNMATCH -11

#define TEMP_BUFF_SIZE 1024
#define RING_BUFF_SIZE 256

#define RING_POINTER_MASK RING_BUFF_SIZE -1 
#define BOTH_INIT 0
#define READER_INIT 1
#define WRITER_INIT 2

/// struct of ring buffer used on shared memory
struct ring_buff{
    unsigned int rp;            ///< read pointer
    unsigned int wp;            ///< write pointer
    char buff[RING_BUFF_SIZE];  ///< ring buffer body
};

int calc_data_size( struct ring_buff *rb);
int calc_rest_size( struct ring_buff *rb);

void set_ope_code( unsigned char ope_code);
void set_byte_code( unsigned char code);
void set_short_code( short int code );
void set_int_code( int code);
void set_long_code( long code);

void get_ope_code( void );
void get_byte_code( void );
void get_short_code( void );
void get_int_code( void );
void get_long_code( void );

void copy_str( char* dest, char *src, int len);
//void save_str( int len );
int put_command( struct ring_buff *rb, char *str, int len );

void dump_buff(void);

void init_ring_buff(struct ring_buff *rb, int init_type);
int get_ring_buff(struct ring_buff *rb, char *buff, int max_len);
int put_ring_buff(struct ring_buff *rb, char *str, int len );

int context_switch( int max_count);


#ifdef	__cplusplus
}
#endif

#endif	/* COMMAND_CODE_H */

