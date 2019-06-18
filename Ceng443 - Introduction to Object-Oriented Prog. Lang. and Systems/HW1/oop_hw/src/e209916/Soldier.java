package e209916;

/**
 * Base class for all soldier types.
 */
public abstract class Soldier extends SimulationObject {
	
	private final SoldierType soldiertype;
	private final double collisionRange;
	private final double shootingRange;
	private SoldierState soldierState;
	
	private final double bulletSpeed;

	/**
	 * Constructs a new Soldier with the given name, position, speed, soldier type,
	 * collision range, detection range, soldier state, bullet speed.
	 * @param name The name of the soldier.
	 * @param position The position of the soldier.
	 * @param speed The speed of the soldier.
	 * @param soldierType The type of the soldier.
	 * @param collisionRange The collision range of the soldier.
	 * @param shootingRange The detection range for the soldier.
	 * @param soldierState The state of the soldier.
	 * @param bulletSpeed The speed of the bullet which soldier fires.
	 */
	public Soldier(String name, Position position, double speed, SoldierType soldierType,
				   double collisionRange, double shootingRange, SoldierState soldierState,
				   double bulletSpeed) {
		super(name, position, speed);
		this.soldiertype = soldierType;
		this.collisionRange = collisionRange;
		this.shootingRange = shootingRange;
		this.soldierState = soldierState;
		this.bulletSpeed = bulletSpeed;
	}

	/**
	 * Visits the given SimulationObjectVisitor.
	 * @param visitor The SimulationObjectVisitor which will be visited.
	 */
	@Override
    public void accept(SimulationObjectVisitor visitor) {
    	visitor.visit(this);
    }

	/**
	 * Processes step behaviour of the soldier.
	 * @param controller The SimulationController which Soldier step behaviour is processed.
	 */
	@Override
	public void step(SimulationController controller) {
		if(getSoldierState() == SoldierState.SEARCHING)
			searching(controller);
		else if(getSoldierState() == SoldierState.SHOOTING)
			shooting(controller);
		else if(getSoldierState() == SoldierState.AIMING){
			aiming(controller);
		}
	}

	/**
	 * Processes step behaviour of the soldier when its state is searching.
	 * @param controller The SimulationController which Soldier step behaviour is processed.
	 */
	protected abstract void searching(SimulationController controller);

	/**
	 * Processes step behaviour of the soldier when its state is aiming.
	 * @param controller The SimulationController which Soldier step behaviour is processed.
	 */
	protected void aiming(SimulationController controller) {
		Zombie closestZombie = findClosestObject(controller.getZombies());
		double distance = getPosition().distance(closestZombie.getPosition());
		if(distance <= getShootingRange()) {
			setDirectionToTheTarget(closestZombie);

			setSoldierState(SoldierState.SHOOTING);
		}
		else {
			setSoldierState(SoldierState.SEARCHING);
		}
	}

	/**
	 * Processes step behaviour of the soldier when its state is shooting.
	 * @param controller The SimulationController which Soldier step behaviour is processed.
	 */
	protected void shooting(SimulationController controller) {
		fireBullet(controller);

		double distance = distanceToTheClosestObject(controller.getZombies());
		if(distance <= getShootingRange()) {
			setSoldierState(SoldierState.AIMING);
		}
		else {
			setDirection(Position.generateRandomDirection(true));
			setSoldierState(SoldierState.SEARCHING);
		}	
	}

	/**
	 * Fires a bullet at soldier position from soldier direction.
	 * @param controller The SimulationController which Soldier step behaviour is processed.
	 */
	protected void fireBullet(SimulationController controller) {
		Bullet newBullet = new Bullet(Bullet.getBulletNo(), getPosition(), bulletSpeed);
		newBullet.setDirection(getDirection());
		
		controller.addSimulationObject(newBullet);
		Bullet.incrementBulletNo();
		
        System.out.printf("%s fired %s to direction %s.%n", getName(), newBullet.getName(), getDirection());
	}

	/**
	 * Sets the direction of the soldier as the given position.
	 * @param direction The desired direction for the soldier.
	 */
	@Override
    public void setDirection(Position direction) {
        super.setDirection(direction);
        System.out.printf("%s changed direction to %s.%n", getName(), direction);
    }

	/**
	 * Sets the position of the soldier as the given position.
	 * @param position The desired position for the soldier.
	 */
	@Override
    public void setPosition(Position position) {
        super.setPosition(position);
        System.out.printf("%s moved to %s.%n", getName(), position);
    }

	/**
	 * Gets the state of the soldier.
	 * @return soldierState The state of the soldier.
	 */
	public SoldierState getSoldierState() {
		return soldierState;
	}

	/**
	 * Sets the state of the soldier as the given state.
	 * @param soldierState The desired state for the soldier.
	 */
	public void setSoldierState(SoldierState soldierState) {
		this.soldierState = soldierState;
        System.out.printf("%s changed state to %s.%n", getName(), soldierState);
	}

	/**
	 * Gets the collision range of the soldier.
	 * @return collisionRange The collision range of the soldier.
	 */
	public double getCollisionRange() {
		return collisionRange;
	}

	/**
	 * Gets the shooting range of the soldier.
	 * @return shootingRange The shooting range of the soldier.
	 */
	public double getShootingRange() {
		return shootingRange;
	}

}
