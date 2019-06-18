#include "GameEngine.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

GameEngine::GameEngine(uint boardSize, std::vector<Player *> *players) : board(Board(boardSize, players))
{
	currentRound = 1;
	c = 0;
}


GameEngine::~GameEngine()
{
	while(board.getPlayer()->size() != 0)
	{
		Player *a = (*board.getPlayer())[0];
		board.getPlayer()->erase(board.getPlayer()->begin());
		delete a;
	}
	delete board.getPlayer();
}


const Board& GameEngine::getBoard() const
{
	return board;
}


Player* GameEngine::operator[](uint id) const
{	
	for (int i = 0; i < board.getPlayer()->size(); i++)
	{
		if((*board.getPlayer())[i]->getID() == id)
			return (*board.getPlayer())[i];
	}
	return nullptr;
}


bool GameEngine::isFinished() const
{ 
	if((*board.getPlayer()).size() == 0)
		return true;
	else if((*board.getPlayer()).size() == 1 && board.isCoordHill((*board.getPlayer())[0]->getCoord()))
	{
		return true;
	}
	else
		return false;
}


void GameEngine::takeTurn()
{
	c = 0;

	std::cout << "-- START ROUND " << currentRound << " --" << std::endl;

	board.updateStorm(currentRound);

	for (c = 0; c < board.getPlayer()->size(); c++)
	{
		takeTurnForPlayer((*board.getPlayer())[c]->getID());
	}

	std::cout << "-- END ROUND " << currentRound << " --" << std::endl;
	currentRound++;
}


Move GameEngine::takeTurnForPlayer(uint playerID)
{
	int i = 0, j = 0, k = 0, index = 0, index2 = 0;
	Player *a = nullptr;

	std::vector<Move> pl;
	std::vector<Coordinate> vis;


	for (i = 0; i < board.getPlayer()->size(); i++)
	{
		if((*board.getPlayer())[i]->getID() == playerID)
		{
			a = (*board.getPlayer())[i];
			break;
		}
	}
	index = i;
	if(a == nullptr)
		return NOOP;

	pl = a->getPriorityList();
	vis  = board.visibleCoordsFromCoord(a->getCoord());

	Coordinate hill = Coordinate((board.getSize()-1)/2, (board.getSize()-1)/2);

	if(board.isStormInCoord(a->getCoord()))
	{
		int stormdamage = Entity::stormDamageForRound(currentRound);
		std::cout << a->getFullName() << "(" << a->getHP() << ") is STORMED! (-" << stormdamage << ")" << std::endl;
		a->damageHP(stormdamage);
	}

	if(a->isDead())
	{
		std::cout << a->getFullName() << "(" << a->getHP() << ") DIED." << std::endl;

		board.getPlayer()->erase(board.getPlayer()->begin()+index);
		delete a;

		c--;

		return NOOP;
	}

	for (i = 0; i < pl.size(); i++)
	{
		if(pl[i] == NOOP)
			return NOOP;

		else if(pl[i] == ATTACK)
		{
			Player *attacked = nullptr;
			int min = 100;
			for (k = 0; k < board.getPlayer()->size(); k++)
			{
				for (j = 0; j < vis.size(); j++)
				{
					if((*board.getPlayer())[k]->getCoord() == vis[j])
					{
						if((*board.getPlayer())[k]->getID() < min)
						{
							index2 = k;
							attacked = (*board.getPlayer())[k];
							min = (*board.getPlayer())[k]->getID();
						}
					}

				}
			}

			if(attacked == nullptr)
				continue;

			bool isPlayerDead = a->attackTo(attacked);

			if(isPlayerDead)
			{
				std::cout << attacked->getFullName() << "(" << attacked->getHP() << ") DIED." << std::endl; 
				board.getPlayer()->erase(board.getPlayer()->begin()+index2);
				delete attacked;
				if(index2 < index)
					c--;
			}
			return ATTACK;
		}
		else
		{
			if(a->getCoord() - board.calculateCoordWithMove(pl[i], a->getCoord()) != 0)
			{
				if( (a->getCoord() - hill) > (board.calculateCoordWithMove(pl[i], a->getCoord()) - hill) )
				{
					a->executeMove(pl[i]);
					return pl[i];
				}
				else
					continue;
			}
			else
				continue;
		}
	}
	return NOOP;
}


Player *GameEngine::getWinnerPlayer() const
{
	if(board.getPlayer()->size() == 0)
		return nullptr;
	else if(!isFinished())
		return nullptr;
	else
		return (*board.getPlayer())[0];
}
