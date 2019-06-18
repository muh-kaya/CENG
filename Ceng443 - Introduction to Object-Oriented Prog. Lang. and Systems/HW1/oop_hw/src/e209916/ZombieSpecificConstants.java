package e209916;

import e209916.ZombieState;

/**
 * This class contains some constants values which is specific for
 * Zombie type.
 */
public class ZombieSpecificConstants {

	public static final double REGULAR_ZOMBIE_COLLISION_RANGE = 2.0;
	public static final double REGULAR_ZOMBIE_DETECTION_RANGE = 20.0;
	public static final double REGULAR_ZOMBIE_SPEED = 5.0;
	public static final ZombieState REGULAR_ZOMBIE_DEFAULT_STATE = ZombieState.WANDERING;
	
	public static final double SLOW_ZOMBIE_COLLISION_RANGE = 1.0;
	public static final double SLOW_ZOMBIE_SHOOTING_RANGE = 40.0;
	public static final double SLOW_ZOMBIE_SPEED = 2.0;
	public static final ZombieState SLOW_ZOMBIE_DEFAULT_STATE = ZombieState.WANDERING;
	
	public static final double FAST_ZOMBIE_COLLISION_RANGE = 2.0;
	public static final double FAST_ZOMBIE_SHOOTING_RANGE = 20.0;
	public static final double FAST_ZOMBIE_SPEED = 20.0;
	public static final ZombieState FAST_ZOMBIE_DEFAULT_STATE = ZombieState.WANDERING;
}
