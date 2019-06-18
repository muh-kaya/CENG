package e209916;

/**
 * This is class for fast type of zombie.
 */
public class FastZombie extends Zombie {

	/**
	 * Constructs a new FastZombie with the given name, position and some FastZombie specific
	 * constants.
	 * @param name The name of the FastZombie.
	 * @param position The position of the FastZombie.
	 */
    public FastZombie(String name, Position position) { // DO NOT CHANGE PARAMETERS
        super(name, position, ZombieSpecificConstants.FAST_ZOMBIE_SPEED,
        		ZombieType.FAST, ZombieSpecificConstants.FAST_ZOMBIE_COLLISION_RANGE,
        		ZombieSpecificConstants.FAST_ZOMBIE_SHOOTING_RANGE, 
        		ZombieSpecificConstants.FAST_ZOMBIE_DEFAULT_STATE);
    }

	/**
	 * Processes step behaviour of the FastZombie when its state is wandering.
	 * @param controller The SimulationController which FastZombie step behaviour is processed.
	 */
	@Override
	protected void wander(SimulationController controller) {
		Soldier closestSoldier = findClosestObject(controller.getSoldiers());
		double distance = getPosition().distance(closestSoldier.getPosition());
		if(distance <= getDetectionRange()) {
			setDirectionToTheTarget(closestSoldier);
			
			setZombieState(ZombieState.FOLLOWING);
			return;
		}
		else {
			super.setNextMovement(controller);
		}
	}

	/**
	 * Processes step behaviour of the FastZombie when its state is following.
	 * @param controller The SimulationController which FastZombie step behaviour is processed.
	 */
	@Override
	protected void follow(SimulationController controller) {
		super.setNextMovement(controller);

		setZombieState(ZombieState.WANDERING);
	}

}
