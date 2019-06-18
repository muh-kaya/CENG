package e209916;

/**
 * This class for adding SimulationObject to the different array list without checking
 * their types.
 */
public class AddSimulationObjectVisitor implements SimulationObjectVisitor {
	private SimulationController simulationController;

	/**
	 * Constructs a new AddSimulationObjectVisitor with the given SimulationController.
	 * @param simulationController The controller which visitor works on.
	 */
	public AddSimulationObjectVisitor(SimulationController simulationController) {
		this.simulationController = simulationController;
	}

	/**
	 * Calls add method on the SimulationController with the given soldier.
	 * @param soldier The soldier that will be added to the soldiers array list.
	 */
	@Override
	public void visit(Soldier soldier) {
		simulationController.add(soldier);
	}

	/**
	 * Calls add method on the SimulationController with the given zombie.
	 * @param zombie The soldier that will be added to the zombies array list.
	 */
	@Override
	public void visit(Zombie zombie) {
		simulationController.add(zombie);
	}

	/**
	 * Calls add method on the SimulationController with the given bullet.
	 * @param bullet The soldier that will be added to the bullets array list.
	 */
	@Override
	public void visit(Bullet bullet) {
		simulationController.add(bullet);
	}

}
