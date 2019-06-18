package e209916;

/**
 * This is class for Slow type of zombie.
 */
public class SlowZombie extends Zombie{

	/**
	 * Constructs a new SlowZombie with the given name, position and some SlowZombie specific
	 * constants.
	 * @param name The name of the SlowZombie.
	 * @param position The position of the SlowZombie.
	 */
    public SlowZombie(String name, Position position) { // DO NOT CHANGE PARAMETERS
        super(name, position, ZombieSpecificConstants.SLOW_ZOMBIE_SPEED, ZombieType.SLOW,
        		ZombieSpecificConstants.SLOW_ZOMBIE_COLLISION_RANGE, 
        		ZombieSpecificConstants.SLOW_ZOMBIE_SHOOTING_RANGE, 
        		ZombieSpecificConstants.SLOW_ZOMBIE_DEFAULT_STATE);
    }

	/**
	 * Processes step behaviour of the SlowZombie when its state is wandering.
	 * @param controller The SimulationController which SlowZombie step behaviour is processed.
	 */
	@Override
	protected void wander(SimulationController controller) {
		
		double distance = distanceToTheClosestObject(controller.getSoldiers());
		if(distance <= getDetectionRange()) {
			setZombieState(ZombieState.FOLLOWING);
			return;
		}
		else {
			super.setNextMovement(controller);
		}
	}

	/**
	 * Processes step behaviour of the SlowZombie when its state is following.
	 * @param controller The SimulationController which SlowZombie step behaviour is processed.
	 */
	@Override
	protected void follow(SimulationController controller) {
		Soldier closestSoldier = findClosestObject(controller.getSoldiers());
		double distance = getPosition().distance(closestSoldier.getPosition());
		if(distance <= getDetectionRange()) {
			setDirectionToTheTarget(closestSoldier);			
		}
		super.setNextMovement(controller);
		if(distance <= getDetectionRange()) {
			setZombieState(ZombieState.WANDERING);
		}
	}
}
