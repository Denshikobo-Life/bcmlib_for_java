import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import dkl.bcm2835;
 
public class I2c {

    public static void main(String[] args) {
        String send_string = "Send I2C";
        byte[] receive_buff = new byte[16];
        
        bcm2835.bi_init(args[0]);
        bcm2835.i2c_begin();
        bcm2835.i2c_setSlaveAddress( (byte)0x30 );
        bcm2835.i2c_set_baudrate( 100000 );
        bcm2835.ope_sync();
        byte ret = bcm2835.i2c_write( send_string, send_string.length() );
        System.out.print("i2c_write ="+(int)ret+"\n");
        ret = bcm2835.i2c_read( receive_buff, 10 );
        String rec;
        if( ret == 0 )try {
            rec = new String( receive_buff , "UTF-8");
            System.out.print("I2C receive="+rec +"\n");
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger(I2c.class.getName()).log(Level.SEVERE, null, ex);
            rec = "";
        }
        System.out.print("i2c_read ="+(int)ret+"\n");
        bcm2835.i2c_end();
        bcm2835.bi_close();
    }
}
