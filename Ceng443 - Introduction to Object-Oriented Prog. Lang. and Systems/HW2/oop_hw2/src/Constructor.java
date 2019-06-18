import java.util.concurrent.TimeUnit;

public class Constructor extends SmelterConstructor implements Runnable {

    public Constructor(int id, int ingotType, int capacity, int interval) {
        super(id, interval, ingotType, capacity);
    }

    @Override
    public void run() {
        HW2Logger.WriteOutput(0, 0, getId(), Action.CONSTRUCTOR_CREATED);
        try{
            while (true) {

                boolean acquired = getsFull().tryAcquire(getIngotType() + 2, 3, TimeUnit.SECONDS);

                if(!acquired) {
                    getIsActive().set(false);
                    break;
                }
                HW2Logger.WriteOutput(0, 0, getId(), Action.CONSTRUCTOR_STARTED);

                sleep();

                getsEmpty().release(getIngotType() + 2);

                HW2Logger.WriteOutput(0, 0, getId(), Action.CONSTRUCTOR_FINISHED);

            }
            HW2Logger.WriteOutput(0, 0, getId(), Action.CONSTRUCTOR_STOPPED);

        } catch (InterruptedException ie){}

    }
}
