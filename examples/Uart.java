import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import dkl.bcm2835;
 
public class Uart {

    public static void main(String[] args) {
        String send_string = "Send Uart";
        byte[] receive_buff = new byte[16];
        String rec;
      
        bcm2835.bi_init(args[0]);
        bcm2835.ope_sync();
        int ret = bcm2835.ope_open_uart();
        System.out.print("open_uart ="+ret+"\n");
        bcm2835.ope_config_uart( 115200 );
        ret = bcm2835.ope_receive_uart(receive_buff, 10);
        System.out.print("receive_uart ="+ret+"\n");
        ret = bcm2835.ope_send_uart(send_string, send_string.length());
        System.out.print("send_uart ="+ret+"\n");
        ret = bcm2835.ope_receive_uart(receive_buff, 10);
        System.out.print("receive_uart ="+ret+"\n");
        if( ret > 0 )try {
            rec = new String( receive_buff , "UTF-8");
            System.out.print("I2C receive="+rec +"\n");
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger(I2c.class.getName()).log(Level.SEVERE, null, ex);
            rec = "";
        }
        bcm2835.ope_close_uart();
        bcm2835.bi_close();
    }
}
