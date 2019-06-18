package e209916;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Main class for running the simulation.
 */
public class SimulationRunner {

    /**
     * Starts the simulation with creating new
     * simulationController object.
     * @param
     */
    public static void main(String[] args) {
        SimulationController simulation = new SimulationController(50, 50);

        
        simulation.addSimulationObject(new RegularSoldier("Soldier1", new Position(10, 10)));
        simulation.addSimulationObject(new RegularZombie("Zombie1", new Position(40, 40)));
        

        while (!simulation.isFinished()) {
            simulation.stepAll();
            try {
                Thread.sleep(1000);
            } catch (InterruptedException ex) {
                Logger.getLogger(SimulationRunner.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
