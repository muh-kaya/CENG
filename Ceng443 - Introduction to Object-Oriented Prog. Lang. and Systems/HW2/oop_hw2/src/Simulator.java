import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Simulator {

    public static void main(String[] args) {

        ArrayList<Smelter> smelterList = new ArrayList<>();
        ArrayList<Constructor> constructorList = new ArrayList<>();
        ArrayList<Transporter> transporterList = new ArrayList<>();

        Scanner scanner = new Scanner(System.in);
        int numberOfSmelters = scanner.nextInt();

        HW2Logger.InitWriteOutput();

        for (int i=0; i<numberOfSmelters; i++){
            int interval = scanner.nextInt();
            int capacity = scanner.nextInt();
            int ingotType = scanner.nextInt();
            int totalIngot = scanner.nextInt();
            Smelter smelter = new Smelter(i+1, ingotType, capacity, interval, totalIngot);
            smelterList.add(smelter);
        }

        int numberOfConstructors = scanner.nextInt();

        for (int i=0; i<numberOfConstructors; i++){
            int interval = scanner.nextInt();
            int capacity = scanner.nextInt();
            int ingotType = scanner.nextInt();
            Constructor constructor = new Constructor(i+1, ingotType, capacity, interval);
            constructorList.add(constructor);
        }

        int numberOfTransporters = scanner.nextInt();

        for (int i=0; i<numberOfTransporters; i++){
            int interval = scanner.nextInt();
            int smelterID = scanner.nextInt();
            int constructorID = scanner.nextInt();
            Transporter transporter = new Transporter(i+1, interval, smelterList.get(smelterID-1), constructorList.get(constructorID-1));
            transporterList.add(transporter);
        }

        ExecutorService taskList = Executors.newFixedThreadPool(numberOfSmelters + numberOfTransporters + numberOfConstructors);

        for (int i=0; i<numberOfSmelters; i++){
            taskList.execute(smelterList.get(i));
        }

        for (int i=0; i<numberOfConstructors; i++){
            taskList.execute(constructorList.get(i));
        }

        for (int i=0; i<numberOfTransporters; i++){
            taskList.execute(transporterList.get(i));
        }

        taskList.shutdown();
    }
}