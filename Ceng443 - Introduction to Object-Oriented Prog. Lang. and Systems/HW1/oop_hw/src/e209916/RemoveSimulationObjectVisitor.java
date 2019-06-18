package e209916;

/**
 * This class for removing SimulationObject to the different array list without checking
 * their types.
 */
public class RemoveSimulationObjectVisitor implements SimulationObjectVisitor {
	private SimulationController simulationController;

	/**
	 * Constructs a new AddSimulationObjectVisitor with the given SimulationController.
	 * @param simulationController The controller which visitor works on.
	 */
	public RemoveSimulationObjectVisitor(SimulationController simulationController) {
		this.simulationController = simulationController;
	}

	/**
	 * Calls remove method on the SimulationController with the given soldier.
	 * @param soldier The soldier that will be removed from the soldiers array list.
	 */
	@Override
	public void visit(Soldier soldier) {
		simulationController.remove(soldier);
	}

	/**
	 * Calls remove method on the SimulationController with the given zombie.
	 * @param zombie The soldier that will be removed from the zombies array list.
	 */
	@Override
	public void visit(Zombie zombie) {
		simulationController.remove(zombie);
	}

	/**
	 * Calls remove method on the SimulationController with the given bullet.
	 * @param bullet The soldier that will be removed from the bullets array list.
	 */
	@Override
	public void visit(Bullet bullet) {
		simulationController.remove(bullet);
	}

}
