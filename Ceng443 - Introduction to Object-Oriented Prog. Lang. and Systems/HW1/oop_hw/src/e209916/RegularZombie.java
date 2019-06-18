package e209916;

/**
 * This is class for regular type of zombie.
 */
public class RegularZombie extends Zombie{
	
	private int countInFollowingState = 0;

	/**
	 * Constructs a new RegularZombie with the given name, position and some RegularZombie specific
	 * constants.
	 * @param name The name of the RegularZombie.
	 * @param position The position of the RegularZombie.
	 */
    public RegularZombie(String name, Position position) { // DO NOT CHANGE PARAMETERS
        super(name, position, ZombieSpecificConstants.REGULAR_ZOMBIE_SPEED,
        		ZombieType.REGULAR, ZombieSpecificConstants.REGULAR_ZOMBIE_COLLISION_RANGE,
        		ZombieSpecificConstants.REGULAR_ZOMBIE_DETECTION_RANGE,
        		ZombieSpecificConstants.REGULAR_ZOMBIE_DEFAULT_STATE);
    }

	/**
	 * Processes step behaviour of the RegularZombie when its state is wandering.
	 * @param controller The SimulationController which RegularZombie step behaviour is processed.
	 */
	@Override
	protected void wander(SimulationController controller) {
		
		super.setNextMovement(controller);
		
		Soldier closestSoldier = findClosestObject(controller.getSoldiers());
		double distance = getPosition().distance(closestSoldier.getPosition());
		if(distance <= getDetectionRange()) {
			setDirectionToTheTarget(closestSoldier);
			
			setZombieState(ZombieState.FOLLOWING);
		}
	}

	/**
	 * Processes step behaviour of the RegularZombie when its state is following.
	 * @param controller The SimulationController which RegularZombie step behaviour is processed.
	 */
	@Override
	protected void follow(SimulationController controller) {

		super.setNextMovement(controller);
		
		countInFollowingState++;
		if(countInFollowingState == 4) {
			setZombieState(ZombieState.WANDERING);
			countInFollowingState = 0;
		}
	}
    
}
