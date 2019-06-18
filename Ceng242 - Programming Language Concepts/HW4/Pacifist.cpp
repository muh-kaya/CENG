#include "Pacifist.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
Pacifist::Pacifist(uint id, int x, int y) : Player(id, x, y) 
{
	HP = 100;
}

Armor Pacifist::getArmor() const
{
	return METAL;
}

Weapon Pacifist::getWeapon() const
{
	return NOWEAPON;
}


std::vector<Move> Pacifist::getPriorityList() const
{
	std::vector<Move> v;
	v.push_back(UP);
	v.push_back(LEFT);
	v.push_back(DOWN);
	v.push_back(RIGHT);

	return v;
}


const std::string Pacifist::getFullName() const
{
	return "Pacifist" + getBoardID();
}
