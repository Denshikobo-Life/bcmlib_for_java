import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import dkl.bcm2835;
 
public class Test {

  
    static final byte PIN = bcm2835.RPI_GPIO_P1_11;
    public static void main(String[] args) {

      String send_string = "Test String";
      byte[] receive_buff = new byte[16];
      String reg_addr = "1";

      System.out.print("Test calling BCM2835 Libraries\n");
      bcm2835.bi_init(args[0]);

      bcm2835.set_debug( (byte)1 );
      bcm2835.peri_read( bcm2835.BCM2835_PERI_BASE);
      bcm2835.peri_read_nb(bcm2835.BCM2835_ST_BASE);
      bcm2835.peri_write(bcm2835.BCM2835_GPIO_PADS, 0x12345678 );
      bcm2835.peri_write_nb(bcm2835.BCM2835_CLOCK_BASE,0x12345678);
      bcm2835.peri_set_bits(bcm2835.BCM2835_GPIO_BASE,0x12345678,0x11111111);
      bcm2835.gpio_fsel(bcm2835.RPI_V2_GPIO_P1_03, bcm2835.BCM2835_GPIO_FSEL_INPT);
      bcm2835.gpio_set(bcm2835.RPI_V2_GPIO_P1_05);
      bcm2835.gpio_clr(bcm2835.RPI_V2_GPIO_P1_07);
      bcm2835.gpio_set_multi(0x22334455);
      bcm2835.gpio_clr_multi(0x22334455);
      bcm2835.gpio_lev(bcm2835.RPI_V2_GPIO_P1_08);
      bcm2835.gpio_eds(bcm2835.RPI_V2_GPIO_P1_10);
      bcm2835.gpio_set_eds(bcm2835.RPI_V2_GPIO_P1_11);
      bcm2835.gpio_ren(bcm2835.RPI_V2_GPIO_P1_12);
      bcm2835.gpio_clr_ren(bcm2835.RPI_V2_GPIO_P1_13);
      bcm2835.gpio_fen(bcm2835.RPI_V2_GPIO_P1_15);
      bcm2835.gpio_clr_fen(bcm2835.RPI_V2_GPIO_P1_16);
      bcm2835.gpio_hen(bcm2835.RPI_V2_GPIO_P1_18);
      bcm2835.gpio_clr_hen(bcm2835.RPI_V2_GPIO_P1_19);
      bcm2835.gpio_len(bcm2835.RPI_V2_GPIO_P1_21);
      bcm2835.gpio_clr_len(bcm2835.RPI_V2_GPIO_P1_22);
      bcm2835.gpio_aren(bcm2835.RPI_V2_GPIO_P1_23);
      bcm2835.gpio_clr_aren(bcm2835.RPI_V2_GPIO_P1_24);
      bcm2835.gpio_afen(bcm2835.RPI_V2_GPIO_P1_26);
      bcm2835.gpio_clr_afen(bcm2835.RPI_V2_GPIO_P5_03);
      bcm2835.gpio_pud( bcm2835.BCM2835_GPIO_PUD_UP);
      bcm2835.gpio_pudclk(bcm2835.RPI_V2_GPIO_P5_04, (byte)1);
      bcm2835.gpio_pad(bcm2835.BCM2835_PAD_GROUP_GPIO_0_27);
      bcm2835.gpio_set_pad(bcm2835.BCM2835_PAD_GROUP_GPIO_28_45, bcm2835.BCM2835_PAD_DRIVE_2mA);
//      bcm2835.bi_delay ( 0x12345678 );
//      bcm2835.bi_delayMicroseconds ( 123456789000L );
      bcm2835.gpio_write(bcm2835.RPI_V2_GPIO_P5_05, (byte)1 );
      bcm2835.gpio_write_multi(0x12345678, (byte)1 );
      bcm2835.gpio_write_mask(0x12345678, 0x11223344);
//      bcm2835.gpio_set_pud(bcm2835.RPI_V2_GPIO_P5_06, (byte)1);
      bcm2835.spi_begin();
      bcm2835.spi_end();
      bcm2835.spi_setClockDivider(bcm2835.BCM2835_SPI_CLOCK_DIVIDER_512);
      bcm2835.spi_setDataMode(bcm2835.BCM2835_SPI_MODE0);
      bcm2835.spi_chipSelect(bcm2835.BCM2835_SPI_CS0);
      bcm2835.spi_setChipSelectPolarity(bcm2835.BCM2835_SPI_CS1, (byte)1);
//    bcm2835.spi_transfer((byte)0x12);
//    bcm2835.spi_transfernb(send_string, receive_buff, send_string.length());
//    bcm2835.spi_transfern(send_string, send_string.length());
//      bcm2835.spi_writenb(send_string, send_string.length());
      bcm2835.i2c_begin();
      bcm2835.i2c_end();
      bcm2835.i2c_setSlaveAddress((byte)0x30);
      bcm2835.i2c_setClockDivider(bcm2835.BCM2835_I2C_CLOCK_DIVIDER_2500);
      bcm2835.i2c_set_baudrate( 100000 );
//      bcm2835.i2c_write(send_string, send_string.length());
//      bcm2835.i2c_read( receive_buff, receive_buff.length);
//      bcm2835.i2c_read_register_rs(reg_addr, receive_buff, receive_buff.length);
//      bcm2835.st_read();
//      bcm2835.st_delay(123456789000L, 223456789000L);

      bcm2835.bi_close();
    }
}
