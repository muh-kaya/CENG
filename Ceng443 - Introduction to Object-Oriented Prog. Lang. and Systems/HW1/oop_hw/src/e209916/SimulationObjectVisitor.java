package e209916;

/**
 * An interface which can visit all type of SimulationObjects.
 */
public interface SimulationObjectVisitor {

	/**
	 * Visit function for soldier type.
	 * @param soldier The soldier to perform action on.
	 */
	void visit(Soldier soldier);

	/**
	 * Visit function for zombie type.
	 * @param zombie The zombie to perform action on.
	 */
	void visit(Zombie zombie);

	/**
	 * Visit function for bullet type.
	 * @param bullet The bullet to perform action on.
	 */
	void visit(Bullet bullet);
}
