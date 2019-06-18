
public class Smelter extends SmelterConstructor implements Runnable {

    private int totalIngot;

    public Smelter(int id, int ingotType, int capacity, int interval, int totalIngot) {
        super(id, interval, ingotType, capacity);
        this.totalIngot = totalIngot;
    }

    @Override
    public void run() {
        int produced = 0;

        HW2Logger.WriteOutput(getId(), 0, 0, Action.SMELTER_CREATED);
        try{
            while (produced < totalIngot){

                    getsEmpty().acquire();

                    HW2Logger.WriteOutput(getId(), 0 , 0, Action.SMELTER_STARTED);

                    sleep();

                    produced++;

                    if(produced == totalIngot){
                        getIsActive().set(false);
                    }

                    getsFull().release();

                    getStoredIngots().incrementAndGet();

                    HW2Logger.WriteOutput(getId(), 0 , 0, Action.SMELTER_FINISHED);

                    sleep();
            }
            HW2Logger.WriteOutput(getId(), 0, 0, Action.SMELTER_STOPPED);

        } catch (InterruptedException ie){}
    }
}
