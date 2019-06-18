package e209916;

/**
 * This is class for Sniper type of soldier.
 */
public class Sniper extends Soldier {
	/**
	 * Constructs a new Sniper with the given name, position and some Sniper specific
	 * constants.
	 * @param name The name of the Sniper.
	 * @param position The position of the Sniper.
	 */
    public Sniper(String name, Position position) { // DO NOT CHANGE PARAMETERS
        super(name, position, SoldierSpecificConstants.SNIPER_SPEED,
        		SoldierType.SNIPER, SoldierSpecificConstants.SNIPER_COLLISION_RANGE,
        		SoldierSpecificConstants.SNIPER_SHOOTING_RANGE,
        		SoldierSpecificConstants.SNIPER_SOLDIER_DEFAULT_STATE,
        		SoldierSpecificConstants.SNIPER_BULLET_SPEED);
    }

	/**
	 * Processes step behaviour of the Sniper when its state is searching.
	 * @param controller The SimulationController which Sniper step behaviour is processed.
	 */
	@Override
	protected void searching(SimulationController controller) {
		super.setNextMovement(controller);
		
		setSoldierState(SoldierState.AIMING);	
	}
}
