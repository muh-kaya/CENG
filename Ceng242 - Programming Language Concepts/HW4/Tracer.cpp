#include "Tracer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Tracer::Tracer(uint id, int x, int y) : Player(id, x, y) 
{
	HP = 100;
}

Armor Tracer::getArmor() const
{
	return BRICK;
}

Weapon Tracer::getWeapon() const
{
	return SHOVEL;
}


std::vector<Move> Tracer::getPriorityList() const
{
	std::vector<Move> v;
	v.push_back(UP);
	v.push_back(LEFT);
	v.push_back(DOWN);
	v.push_back(RIGHT);
	v.push_back(ATTACK);

	return v;
}


const std::string Tracer::getFullName() const
{
	return "Tracer" + getBoardID();
}
