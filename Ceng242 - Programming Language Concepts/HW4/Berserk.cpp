#include "Berserk.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Berserk::Berserk(uint id, int x, int y) : Player(id, x, y) 
{
	HP = 100;
}

Armor Berserk::getArmor() const
{
	return WOODEN;
}

Weapon Berserk::getWeapon() const
{
	return PISTOL;
}


std::vector<Move> Berserk::getPriorityList() const
{
	std::vector<Move> v;
	v.push_back(ATTACK);
	v.push_back(UP);
	v.push_back(LEFT);
	v.push_back(DOWN);
	v.push_back(RIGHT);

	return v;
}


const std::string Berserk::getFullName() const
{
	return "Berserk" + getBoardID();
}
