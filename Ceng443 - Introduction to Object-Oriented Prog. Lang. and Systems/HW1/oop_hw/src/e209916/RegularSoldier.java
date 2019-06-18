package e209916;

/**
 * This is class for Regular type of soldier.
 */
public class RegularSoldier extends Soldier {

	/**
	 * Constructs a new RegularSoldier with the given name, position and some RegularSoldier specific
	 * constants.
	 * @param name The name of the RegularSoldier.
	 * @param position The position of the RegularSoldier.
	 */
    public RegularSoldier(String name, Position position) { // DO NOT CHANGE PARAMETERS
    	super(name, position, SoldierSpecificConstants.REGULAR_SOLDIER_SPEED,
    			SoldierType.REGULAR, SoldierSpecificConstants.REGULAR_SOLDIER_COLLISION_RANGE,
    			SoldierSpecificConstants.REGULAR_SOLDIER_SHOOTING_RANGE,
    			SoldierSpecificConstants.REGULAR_SOLDIER_DEFAULT_STATE,
    			SoldierSpecificConstants.REGULAR_SOLDIER_BULLET_SPEED);
    }

	/**
	 * Processes step behaviour of the RegularSoldier when its state is searching.
	 * @param controller The SimulationController which RegularSoldier step behaviour is processed.
	 */
	@Override
	protected void searching(SimulationController controller) {
		super.setNextMovement(controller);
		
		double distance = distanceToTheClosestObject(controller.getZombies());
		if(distance <= getShootingRange()) {
			setSoldierState(SoldierState.AIMING);
		}
	}
}
