package e209916;

/**
 * Represents a bullet which fired by soldiers..
 */
public class Bullet extends SimulationObject {

	private static int bulletNo = 0;

	/**
	 * Constructs a new Bullet with the given name, position and speed.
	 * @param bulletNumber The name of the Bullet set according to bulletNumber, i.e Bullet1.
	 * @param position The position of the Bullet.
	 * @param speed The speed of the Bullet.
	 */
	public Bullet(int bulletNumber, Position position, double speed) {
		super("Bullet" + bulletNumber, position, speed);
	}

	/**
	 * Gets the static Bullet Number. Bullet number increases as soldiers fire bullet.
	 * @return bulletNo The bullet number
	 */
	public static int getBulletNo() {
		return bulletNo;
	}

	/**
	 * Increments bullet number. Called when soldier fires.
	 */
	public static void incrementBulletNo() {
		bulletNo++;
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
	 * Processes step behaviour of the bullet.
	 * @param controller The SimulationController which SimulationObject step processed.
	 */
	@Override
	public void step(SimulationController controller) {
		int steps = (int) (getSpeed()/1.0);
		
		for(int i=0; i<steps; i++) {
			Zombie closestZombie = findClosestObject(controller.getZombies());
			double distance = getPosition().distance(closestZombie.getPosition());
						
			if(distance <= closestZombie.getCollisionRange()) {
				closestZombie.setActive(false);
				this.setActive(false);
				
				System.out.printf("%s hit %s.%n", getName(), closestZombie.getName());
				
				controller.removeSimulationObject(closestZombie);
				controller.removeSimulationObject(this);
				break;
			}
			
			setPosition(getPosition().addPositions(getDirection()));
			
			if(controller.isPositionOutOfBound(getPosition())) {
				this.setActive(false);
				System.out.printf("%s moved out of bounds.%n", getName());
				controller.removeSimulationObject(this);
				break;
			}
		}
	}
}
