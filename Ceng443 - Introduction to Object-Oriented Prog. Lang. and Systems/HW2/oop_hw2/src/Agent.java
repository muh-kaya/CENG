import java.util.Random;
import java.util.stream.DoubleStream;

public class Agent {

    private int id;
    private int interval;

    public Agent(int id, int interval) {
        this.id = id;
        this.interval = interval;
    }

    protected void sleep(){
        try {
            Random random = new Random(System.currentTimeMillis());
            DoubleStream stream;
            stream = random.doubles(1, interval - interval * 0.01, interval + interval * 0.02);
            Thread.sleep((long) stream.findFirst().getAsDouble());
        } catch (InterruptedException ie) {}
    }

    protected int getId() {
        return id;
    }
}
