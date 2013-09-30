import dkl.bcm2835;

public class Blink {
    static final byte PIN = bcm2835.RPI_GPIO_P1_11;
    public static void main(String[] args) {
        System.out.print("Blink 5times\n");
        bcm2835.bi_init(args[0]);
        bcm2835.gpio_fsel(PIN, bcm2835.BCM2835_GPIO_FSEL_OUTP);
        for(int i=0; i<5; i++ )
        {
            bcm2835.gpio_write(PIN, bcm2835.HIGH);
            bcm2835.bi_delay(500);
            bcm2835.gpio_write(PIN, bcm2835.LOW);
            bcm2835.bi_delay(500);
        }
      bcm2835.ope_sync();
      bcm2835.bi_close();
    }
}
