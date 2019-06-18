package e209916;

import java.util.ArrayList;

/**
 * This class provides the base for simulation objects on the simulation (Soldiers, 
 * Zombies and Bullets).
 */
public abstract class SimulationObject {
    private final String name;
    private Position position;
    private Position direction;
    private final double speed;
    private boolean active;
    
    /**
     * Constructs a new SimulationObject with the given name, position and speed.
     * @param name The name of the simulation object.
     * @param position The position of the simulation object.
     * @param speed The speed of the simulation object.
     */
    public SimulationObject(String name, Position position, double speed) {
        this.name = name;
        this.position = position;
        this.speed = speed;
        this.direction = null;
        this.active = true;
    }

	/**
	 * Finds the distance to the closest SimulationObject from among ArrayList of the searched object.
	 * @param searchedObject An ArrayList which composed of searched objects.
	 * @return distance The distance to the closestObject in the given ArrayList.
	 */
	protected <T> double distanceToTheClosestObject(ArrayList<T> searchedObject) {
		double distance = Double.MAX_VALUE;
		for(T object:searchedObject) {
			if(getPosition().distance(((SimulationObject) object).getPosition()) < distance)
				distance = getPosition().distance(((SimulationObject) object).getPosition());
		}
		return distance;
	}

	/**
	 * Finds the closest SimulationObject from among ArrayList of the searched object.
	 * @param searchedObject An ArrayList which composed of searched objects.
	 * @return closestObject The closestObject in the given ArrayList.
	 */
	protected <T> T findClosestObject(ArrayList<T> searchedObject) {
		T closestObject = null;
		double distance = Double.MAX_VALUE;
		for(T object:searchedObject) {
			if(getPosition().distance(((SimulationObject) object).getPosition()) < distance) {
				closestObject = object;
				distance = getPosition().distance(((SimulationObject) object).getPosition());
			}
		}
		return closestObject;
	}

	/**
	 * Sets next movement of the SimulationObject. Calculates the next position of the
	 * SimulationObject according to its position and direction. If next position is out
	 * of the simulation bounds, randomly changes the direction, else sets position as
	 * calculated position.
	 * @param controller The SimulationController which SimulationObject step processed.
	 */
	protected void setNextMovement(SimulationController controller) {
		Position newPosition = getPosition().addPositions(getDirection().multPosition(getSpeed()));

		if(controller.isPositionOutOfBound(newPosition)) {
			setDirection(Position.generateRandomDirection(true));
		}
		else {
			setPosition(newPosition);
		}
	}

	/**
	 * Sets SimulationObject direction to the closest object.
	 * @param closestObject The closest SimulationObject which the direction will be set.
	 */
	protected void setDirectionToTheTarget(SimulationObject closestObject) {
		Position newDirection = new Position(closestObject.getPosition().getX() - getPosition().getX(),
										closestObject.getPosition().getY() - getPosition().getY());
		newDirection.normalize();
		setDirection(newDirection);
	}
	
	/**
	 * Gets the name of the SimulationObject.
	 * @return The name of the SimulationObject.
	 */
    public String getName() {
        return name;
    }

	/**
	 * Gets the position of the SimulationObject.
	 * @return The position of the SimulationObject.
	 */
    public Position getPosition() {
        return position;
    }

	/**
	 * Sets the position of the SimulationObject as the given position.
	 * @param position The desired position for the simulation object.
	 */
    public void setPosition(Position position) {
        this.position = position;
    }

	/**
	 * Gets the direction of the SimulationObject.
	 * @return The direction of the SimulationObject.
	 */
    public Position getDirection() {
        return direction;
    }

	/**
	 * Sets the direction of the SimulationObject as the given position.
	 * @param direction The desired direction for the simulation object.
	 */
    public void setDirection(Position direction) {
        this.direction = direction;
    }

	/**
	 * Gets the speed of the SimulationObject.
	 * @return The speed of the SimulationObject.
	 */
    public double getSpeed() {
        return speed;
    }

	/**
	 * Gets the active value of the SimulationObject.
	 * @return The active value of the SimulationObject.
	 */
    public boolean isActive() {
        return active;
    }

	/**
	 * Sets the active value of the SimulationObject as the given position.
	 * @param active The desired active value for the simulation object.
	 */
    public void setActive(boolean active) {
        this.active = active;
    }

	/**
	 * Processes steps for this SimulationObject.
	 * @param controller The SimulationController which SimulationObject step processed.
	 */
    public abstract void step(SimulationController controller);

	/**
	 * Accepts the SimulationObjectVisitor.
	 * @param visitor The SimulationObjectVisitor which will be visited.
	 */
    public abstract void accept(SimulationObjectVisitor visitor);
}
