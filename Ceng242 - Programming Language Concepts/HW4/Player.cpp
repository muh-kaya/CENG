#include "Player.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Player::Player(uint id, int x, int y) : id(id), coordinate(x, y)
{

}


Player::~Player()
{

}

uint Player::getID() const
{
	return id;
}

const Coordinate& Player::getCoord() const
{
	return coordinate;
}

int Player::getHP() const
{
	return HP;
}

std::string Player::getBoardID() const
{
	if(id < 10)
	{
		return "0" + std::to_string(id);
	}
	else
	{
		return std::to_string(id);
	}
}


bool Player::isDead() const
{
	if(HP <= 0)
		return true;
	else 
		return false;

}


void Player::executeMove(Move move)
{
	coordinate = coordinate + move;
	if(move != ATTACK && move != NOOP)
	{
		std::cout << getFullName() << "(" << getHP() << ") moved ";

		if(move == UP)
			std::cout << "UP" << "." << std::endl;
		else if(move == LEFT)
			std::cout << "LEFT" << "." << std::endl;
		else if(move == RIGHT)
			std::cout << "RIGHT" << "." << std::endl;
		else if(move == DOWN)
			std::cout << "DOWN" << "." << std::endl;
	}
}


bool Player::attackTo(Player *player)
{
	int damage = Entity::damageForWeapon(getWeapon()) - Entity::damageReductionForArmor(player->getArmor());

	if (this == player)
		return this->isDead();

	std::cout << this->getFullName() << "(" << this->getHP() << ") ATTACKED " << player->getFullName() << "(" << player->getHP() << ")! (-" << damage << ")" << std::endl;

	player->HP -= damage;

	return player->isDead();
}

void Player::damageHP(int damage)
{
	this->HP -= damage;
}
