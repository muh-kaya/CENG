package e209916;

import e209916.SoldierState;

/**
 * This class contains some constants values which is specific for
 * Soldier type.
 */
public class SoldierSpecificConstants {

	public static final double REGULAR_SOLDIER_COLLISION_RANGE = 2.0;
	public static final double REGULAR_SOLDIER_SHOOTING_RANGE = 20.0;
	public static final double REGULAR_SOLDIER_SPEED = 5.0;
	public static final double REGULAR_SOLDIER_BULLET_SPEED = 40.0;
	public static final SoldierState REGULAR_SOLDIER_DEFAULT_STATE = SoldierState.SEARCHING;
	
	public static final double COMMANDO_COLLISION_RANGE = 2.0;
	public static final double COMMANDO_SHOOTING_RANGE = 10.0;
	public static final double COMMANDO_SPEED = 10.0;
	public static final double COMMANDO_BULLET_SPEED = 40.0;
	public static final SoldierState COMMANDO_SOLDIER_DEFAULT_STATE = SoldierState.SEARCHING;
	
	public static final double SNIPER_COLLISION_RANGE = 5.0;
	public static final double SNIPER_SHOOTING_RANGE = 40.0;
	public static final double SNIPER_SPEED = 2.0;
	public static final double SNIPER_BULLET_SPEED = 100.0;
	public static final SoldierState SNIPER_SOLDIER_DEFAULT_STATE = SoldierState.SEARCHING;
}
