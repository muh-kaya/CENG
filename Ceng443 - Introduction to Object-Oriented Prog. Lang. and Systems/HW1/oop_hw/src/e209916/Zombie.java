package e209916;

/**
 * Base class for all zombie types.
 */
public abstract class Zombie extends SimulationObject {
	
	private final ZombieType zombietype;
	private final double collisionRange;
	private final double detectionRange;
	private ZombieState zombieState;

	/**
	 * Constructs a new Zombie with the given name, position, speed, zombie type,
	 * collision range, detection range, zombie state, bullet speed.
	 * @param name The name of the zombie.
	 * @param position The position of the zombie.
	 * @param speed The speed of the zombie.
	 * @param zombieType The type of the zombie.
	 * @param collisionRange The collision range of the zombie.
	 * @param detectionRange The detection range of the zombie.
	 * @param zombieState The state of the zombie.
	 */
	public Zombie(String name, Position position, double speed, ZombieType zombieType, 
			double collisionRange, double detectionRange, ZombieState zombieState) {
		super(name, position, speed);
		this.zombietype = zombieType;
		this.collisionRange = collisionRange;
		this.detectionRange = detectionRange;
		this.zombieState = zombieState;
	}

	/**
	 * Processes step behaviour of the zombie.
	 * @param controller The SimulationController which Zombie step behaviour is processed.
	 */
	@Override
	public void step(SimulationController controller) {
		Soldier closestSoldier = findClosestObject(controller.getSoldiers());
		double distance = getPosition().distance(closestSoldier.getPosition());
		
		if(distance <= getCollisionRange() + closestSoldier.getCollisionRange()) {
			closestSoldier.setActive(false);
			controller.removeSimulationObject(closestSoldier);
	        System.out.printf("%s killed %s.%n", getName(), closestSoldier.getName());
			return;
		}
			
		if(getZombieState() == ZombieState.WANDERING)
			wander(controller);
		else if(getZombieState() == ZombieState.FOLLOWING)
			follow(controller);
	}

	/**
	 * Processes step behaviour of the zombie when its state is wandering.
	 * @param controller The SimulationController which Zombie step behaviour is processed.
	 */
	protected abstract void wander(SimulationController controller);

	/**
	 * Processes step behaviour of the zombie when its state is following.
	 * @param controller The SimulationController which Zombie step behaviour is processed.
	 */
	protected abstract void follow(SimulationController controller);

	/**
	 * Visits the given SimulationObjectVisitor.
	 * @param visitor The SimulationObjectVisitor which will be visited.
	 */
	@Override
	public void accept(SimulationObjectVisitor visitor) {
		visitor.visit(this);
	}

	/**
	 * Sets the direction of the zombie as the given position.
	 * @param direction The desired direction for the zombie.
	 */
	@Override
    public void setDirection(Position direction) {
        super.setDirection(direction);
        System.out.printf("%s changed direction to %s.%n", getName(), direction);
    }

	/**
	 * Sets the position of the zombie as the given position.
	 * @param position The desired position for the zombie.
	 */
	@Override
    public void setPosition(Position position) {
        super.setPosition(position);
        System.out.printf("%s moved to %s.%n", getName(), position);
    }

	/**
	 * Gets the state of the zombie.
	 * @return zombieState The state of the zombie.
	 */
	public ZombieState getZombieState() {
		return zombieState;
	}

	/**
	 * Sets the state of the zombie as the given state.
	 * @param zombieState The desired state for the zombie.
	 */
	public void setZombieState(ZombieState zombieState) {
		this.zombieState = zombieState;
		System.out.printf("%s changed state to %s.%n", getName(), zombieState);
	}

	/**
	 * Gets the collision range of the zombie.
	 * @return collisionRange The collision range of the zombie.
	 */
	public double getCollisionRange() {
		return collisionRange;
	}

	/**
	 * Gets the detection range of the zombie.
	 * @return detectionRange The detection range of the zombie.
	 */
	public double getDetectionRange() {
		return detectionRange;
	}

}
