package e209916;

import java.util.ArrayList;

/**
 * This class controls the every event in the simulation.
 */
public class SimulationController {
    private final double height;
    private final double width;
    
    private boolean isFinished = false;
    
    private ArrayList<Soldier> soldiers = new ArrayList<>();
    private ArrayList<Zombie> zombies = new ArrayList<>();
    private ArrayList<Bullet> bullets = new ArrayList<>();
    
    protected ArrayList<SimulationObject> objectsToAdd = new ArrayList<>();
    protected ArrayList<SimulationObject> objectsToRemove = new ArrayList<>();
    
    private AddSimulationObjectVisitor addVisitor = new AddSimulationObjectVisitor(this);
    private RemoveSimulationObjectVisitor removeVisitor = new RemoveSimulationObjectVisitor(this);

	/**
	 * Constructs a new SimulationController with the given width and heigth
	 * @param width The width of the Simulation
	 * @param height The height of the Simulation
	 */
	public SimulationController(double width, double height) {
        this.width = width;
        this.height = height;
    }

	/**
	 * Plays the simulation one step.
	 */
	public void stepAll() {
    	
    	for(Soldier soldier:soldiers) {
    		if(soldier.getDirection() == null) {
    			soldier.setDirection(Position.generateRandomDirection(true));
    		}
    		if(soldier.isActive())
    			soldier.step(this);
    	}
    	
    	for(Zombie zombie:zombies) {
    		if(zombie.getDirection() == null) {
    			zombie.setDirection(Position.generateRandomDirection(true));
    		}
    		if(zombie.isActive())
    			zombie.step(this);
    	}
    	
    	for(Bullet bullet:bullets) {
    		if(bullet.isActive())
    			bullet.step(this);
    	}
    	
    	for(SimulationObject object: objectsToRemove) {
    		remove(object);
    	}
    	objectsToRemove.clear();
    	
    	for(Bullet bullet:bullets) {
    		if(bullet.isActive())
    			System.out.printf("%s dropped to the ground at %s.%n", bullet.getName(), bullet.getPosition());
    	}

    	bullets.clear();
    	
    	
    	for(SimulationObject object: objectsToAdd) {
    		add(object);
    	}
    	objectsToAdd.clear();
    	    	
        if(zombies.size() == 0 || soldiers.size() == 0)
        	isFinished = true;
        else
        	isFinished = false;
    	
    }

	/**
	 * Adds new Object to the simulation.
	 * @param obj The object which will be added to the simulation.
	 */
	public void addSimulationObject(SimulationObject obj) {
    	objectsToAdd.add(obj);
    }

	/**
	 * In the given object, accepts the AddVisitor.
	 * @param obj The object that accepts addVisitor.
	 */
	private void add(SimulationObject obj) {
		obj.accept(addVisitor);
	}

	/**
	 * Adds soldier to the soldier list.
	 * @param soldier The soldier which will be added to list.
	 */
    public void add(Soldier soldier) {
        soldiers.add(soldier);
    }

	/**
	 * Adds zombie to the zombie list.
	 * @param zombie The zombie which will be added to list.
	 */
    public void add(Zombie zombie) {
        zombies.add(zombie);
    }

	/**
	 * Adds bullet to the bullet list.
	 * @param bullet The bullet which will be added to list.
	 */
    public void add(Bullet bullet) {
        bullets.add(bullet);
    }

	/**
	 * Removes Object from the simulation.
	 * @param obj The object which will be removed from the simulation.
	 */
    public void removeSimulationObject(SimulationObject obj) {
    	objectsToRemove.add(obj);
    }

	/**
	 * In the given object, accepts the removeVisitor.
	 * @param obj The object that accepts removeVisitor.
	 */
    private void remove(SimulationObject obj) {
    	obj.accept(removeVisitor);
	}

	/**
	 * Removes soldier from the soldier list.
	 * @param soldier The soldier which will be removed from the list.
	 */
    public void remove(Soldier soldier) {
        soldiers.remove(soldier);
    }

	/**
	 * Removes zombie from the zombie list.
	 * @param zombie The zombie which will be removed from the list.
	 */
    public void remove(Zombie zombie) {
        zombies.remove(zombie);
    }

	/**
	 * Removes bullet from the bullet list.
	 * @param bullet The bullet which will be removed from the list.
	 */
    public void remove(Bullet bullet) {
        bullets.remove(bullet);
    }

	/**
	 * Gets isFinished flag.
	 * @return isFinished The flag that controls game ending.
	 */
	public boolean isFinished() {
    	return isFinished;
    }

	/**
	 * Checks whether position is in bound of simulation or not.
	 * @param p The position which will be checked.
	 * @return true or false
	 */
	public boolean isPositionOutOfBound(Position p) {
    	if(p.getX() < 0 || p.getX() > width)
    		return true;
    	else if(p.getY() < 0 || p.getY() > height)
    		return true;
    	else
    		return false;
    }

	/**
	 * Gets list of soldiers
	 * @return soldiers The list of the soldiers
	 */
	public ArrayList<Soldier> getSoldiers() {
		return soldiers;
	}

	/**
	 * Gets list of zombies
	 * @return zombies The list of the zombies
	 */
	public ArrayList<Zombie> getZombies() {
		return zombies;
	}

}
