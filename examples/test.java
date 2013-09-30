package dkl.example.test;

import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import dkl.bcm2835;
 
public class test {

    static final byte PIN = bcm2835.RPI_GPIO_P1_11;
    public static void main(String[] args) {
        String send_string = "Send I2C";
        byte[] receive_buff = new byte[16];
        
        bcm2835.bi_init(args[0]);
        bcm2835.gpio_fsel(PIN, bcm2835.BCM2835_GPIO_FSEL_OUTP);
        for(int i=0; i<5; i++ )
        {
            bcm2835.gpio_write(PIN, bcm2835.HIGH);
            bcm2835.bi_delay(500);
            bcm2835.gpio_write(PIN, bcm2835.LOW);
            bcm2835.bi_delay(500);
        }
        if( bcm2835.ope_sync() != 0 )
        {
            System.out.print("ope_sync error\n");
        }
        bcm2835.i2c_begin();
        bcm2835.i2c_setSlaveAddress( (byte)0x30 );
        bcm2835.i2c_set_baudrate( 100000 );
        byte ret = bcm2835.i2c_write( send_string, send_string.length() );
        System.out.print("i2c_write ="+(int)ret+"\n");
        bcm2835.i2c_read( receive_buff, 10 );
        bcm2835.i2c_end();
        String rec;
        try {
            rec = new String( receive_buff , "UTF-8");
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger(Jna_Bcm.class.getName()).log(Level.SEVERE, null, ex);
            rec = "";
        }
        System.out.print("I2C receive="+rec +"\n");
        bcm2835.bi_close();
    }
}
