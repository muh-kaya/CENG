package e209916;

import java.util.Random;

/**
 * This class for detecting position of SimulationObjects.
 */
public class Position {
    private double x;
    private double y;
    
    private double length;

    /**
     * Constructs a new position with given x, y values
     * @param x The x coordinate of the position
     * @param y The x coordinate of the position
     */
    public Position(double x, double y) {
        this.x = x;
        this.y = y;
        
        this.calculateLength();
    }

    /**
     * Gets x coordinate of the position
     * @return The x coordinate
     */
    public double getX() {
        return x;
    }

    /**
     * Gets y coordinate of the position
     * @return The y coordinate
     */
    public double getY() {
        return y;
    }

    /**
     * Gets distance between two positions
     * @param other The position which distance calculated to.
     * @return The distance between positions
     */
    public double distance(Position other) {
        return Math.sqrt(Math.pow(this.x-other.getX(), 2)+Math.pow(this.y-other.getY(), 2));
    }

    /**
     * Adds some other position to this position
     * @param other The position which added to.
     */
    public void add(Position other) {
        this.x += other.x;
        this.y += other.y;
        
        this.calculateLength();
    }

    /**
     * Adds some other position to this position
     * @param constant The constant value to multiply.
     */
    public void mult(double constant) {
        this.x *= constant;
        this.y *= constant;
        
        this.calculateLength();
    }

    /**
     * Calculates the length of the position.
     */
    private void calculateLength() {
        this.length = Math.sqrt(Math.pow(x, 2.0)+Math.pow(y, 2.0));
    }

    /**
     * Normalizes the position.
     */
    public void normalize() {
        this.x /= this.length;
        this.y /= this.length;
        
        this.length = 1.0;
    }

    /**
     * Adds the positions and returns result as a new position.
     * @param p The position which will be added.
     * @return The new position which is the result of the addition.
     */
    protected Position addPositions(Position p) {
    	Position result = new Position(this.x + p.x, this.y + p.y);
    	result.calculateLength();
    	
    	return result;
    }

    /**
     * Multiplies position with a constant and returns
     * result as a new position.
     * @param constant The constant value which position will
     * be multiplied.
     * @return The new position which is the result of the multiplication.
     */
    protected Position multPosition(double constant) {
    	Position result = new Position(this.x * constant, this.y * constant);
    	result.calculateLength();
    	
    	return result;
    }

    /**
     * Generates a random Position.
     * @param normalize It specifies that random position will be normalized or not.
     * @return result The random generated Position.
     */
    public static Position generateRandomDirection(boolean normalize) {
        Random random = new Random();
        double x = -1+random.nextDouble()*2;
        double y = -1+random.nextDouble()*2;
        
        Position result = new Position(x, y);
        if (normalize)
            result.normalize();
        return result;
    }
    
    @Override
    protected Object clone() {
        return new Position(x, y); 
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Position other = (Position) obj;
        if (this.x != other.x) {
            return false;
        }
        if (this.y != other.y) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 59 * hash + (int) (Double.doubleToLongBits(this.x) ^ (Double.doubleToLongBits(this.x) >>> 32));
        hash = 59 * hash + (int) (Double.doubleToLongBits(this.y) ^ (Double.doubleToLongBits(this.y) >>> 32));
        return hash;
    }

    @Override
    public String toString() {
        return String.format("(%.2f, %.2f)", x, y);
    }
    
}
