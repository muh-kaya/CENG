package e209916;

/**
 * This is class for Commando type of soldier.
 */
public class Commando extends Soldier {

	/**
	 * Constructs a new Commando with the given name, position and some Commando specific
	 * constants.
	 * @param name The name of the Commando.
	 * @param position The position of the Commando.
	 */
    public Commando(String name, Position position) { // DO NOT CHANGE PARAMETERS
    	super(name, position, SoldierSpecificConstants.COMMANDO_SPEED, SoldierType.COMMANDO,
    			SoldierSpecificConstants.COMMANDO_COLLISION_RANGE,
    			SoldierSpecificConstants.COMMANDO_SHOOTING_RANGE,
    			SoldierSpecificConstants.COMMANDO_SOLDIER_DEFAULT_STATE,
    			SoldierSpecificConstants.COMMANDO_BULLET_SPEED);
    }

	/**
	 * Processes step behaviour of the Commando when its state is searching.
	 * @param controller The SimulationController which Commando step behaviour is processed.
	 */
	@Override
	protected void searching(SimulationController controller) {
		
		Zombie closestZombie = findClosestObject(controller.getZombies());
		double distance = getPosition().distance(closestZombie.getPosition());
		if(distance <= getShootingRange()) {
			setDirectionToTheTarget(closestZombie);
			
			setSoldierState(SoldierState.SHOOTING);
			return;
		}
		
		super.setNextMovement(controller);
		
		closestZombie = findClosestObject(controller.getZombies());
		distance = getPosition().distance(closestZombie.getPosition());
		if(distance <= getShootingRange()) {
			setDirectionToTheTarget(closestZombie);
			setSoldierState(SoldierState.SHOOTING);
		}
	}

	/**
	 * Processes step behaviour of the Commando when its state is shooting.
	 * @param controller The SimulationController which Commando step behaviour is processed.
	 */
	@Override
	protected void shooting(SimulationController controller) {
		fireBullet(controller);

		Zombie closestZombie = findClosestObject(controller.getZombies());
		double distance = getPosition().distance(closestZombie.getPosition());
		if(distance <= getShootingRange()) {
			setDirectionToTheTarget(closestZombie);
		}
		else {
			setDirection(Position.generateRandomDirection(true));
			setSoldierState(SoldierState.SEARCHING);
		}
	}
}
