
public class Transporter extends Agent implements Runnable{

    private Smelter smelter;
    private Constructor constructor;

    public Transporter(int id, int interval, Smelter smelter, Constructor constructor) {
        super(id, interval);
        this.smelter = smelter;
        this.constructor = constructor;
    }

    @Override
    public void run() {
        HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_CREATED);
        try{
            while (constructor.getIsActive().get()){

                /*(smelter.getIsActive().get() || smelter.getSmelterIngots().get() > 0)
                        &&
                        smelter.getsFull().acquire();*/
                boolean flag = smelter.getsFull().tryAcquire();
                if(!flag){
                    if(smelter.getIsActive().get()){
                        continue;
                    }
                    else{
                        break;
                    }
                }

                HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_TRAVEL);

                sleep();

                HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_TAKE_INGOT);

                sleep();

                smelter.getStoredIngots().decrementAndGet();

                smelter.getsEmpty().release();

                constructor.getsEmpty().acquire();

                HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_TRAVEL);

                sleep();

                HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_DROP_INGOT);

                sleep();

                constructor.getsFull().release();

            }
            HW2Logger.WriteOutput(0, getId(), 0, Action.TRANSPORTER_STOPPED);

        } catch (InterruptedException ie){}
    }
}
