import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class SmelterConstructor extends Agent {

    private int ingotType;

    private final Semaphore sEmpty;
    private final Semaphore sFull;

    private AtomicBoolean isActive;
    private AtomicInteger storedIngots;


    public SmelterConstructor(int id, int interval, int ingotType, int capacity) {
        super(id, interval);
        this.ingotType = ingotType;
        this.sEmpty = new Semaphore(capacity);
        this.sFull = new Semaphore(0);
        this.isActive = new AtomicBoolean(true);
        this.storedIngots = new AtomicInteger(0);
    }

    public Semaphore getsEmpty() {
        return sEmpty;
    }

    public Semaphore getsFull() {
        return sFull;
    }

    public int getIngotType() {
        return ingotType;
    }

    public AtomicBoolean getIsActive() {
        return isActive;
    }

    public AtomicInteger getStoredIngots() {
        return storedIngots;
    }
}
