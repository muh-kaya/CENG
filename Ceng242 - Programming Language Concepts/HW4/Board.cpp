#include "Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Board::Board(uint boardSize, std::vector<Player *> *players) : boardSize(boardSize)
{
	this->players = players;
	stormwidth = 0;
}


Board::~Board()
{

}


uint Board::getSize() const
{
	return boardSize;
}


bool Board::isCoordInBoard(const Coordinate& coord) const
{
	if(coord.x < 0 || coord.y < 0 || coord.x >= boardSize || coord.y >= boardSize)
		return false;
	else
		return true;
}


bool Board::isStormInCoord(const Coordinate &coord) const
{
	if(isCoordHill(coord))
		return false;

	if(!isCoordInBoard(coord))
		return false;

	if(coord.x > (stormwidth-1) && coord.y > (stormwidth-1) && coord.x < (boardSize-stormwidth) && coord.y < (boardSize-stormwidth))
	{
		return false;
	}
	return true;
}


bool Board::isCoordHill(const Coordinate& coord) const
{
	if(coord.x == (boardSize-1)/2 && coord.y == (boardSize-1)/2)
		return true;
	return false;
}


Player *Board::operator[](const Coordinate& coord) const
{
	if(!isCoordInBoard(coord))
		return nullptr;

	for (int i = 0; i < players->size(); ++i)
	{
		if((*players)[i]->getCoord() == coord)
			return (*players)[i];
	}
	return nullptr;
}


Coordinate Board::calculateCoordWithMove(Move move, const Coordinate &coord) const
{
	Coordinate newcoord = coord + move;

	if(!isCoordInBoard(newcoord))
	{
		return coord;
	}
	else
	{
		for (int i = 0; i < players->size(); ++i)
		{
			if((*players)[i]->getCoord() == newcoord)
				return coord;
		}
		return newcoord;
	}
}


std::vector<Coordinate> Board::visibleCoordsFromCoord(const Coordinate &coord) const
{
	std::vector<Coordinate> coords;

	if(!isCoordInBoard(coord))
		return coords;
	
	Coordinate newcoord = coord + UP;
	if(isCoordInBoard(newcoord))
		coords.push_back(newcoord);

	newcoord = coord + DOWN;
	if(isCoordInBoard(newcoord))
		coords.push_back(newcoord);

	newcoord = coord + LEFT;
	if(isCoordInBoard(newcoord))
		coords.push_back(newcoord);

	newcoord = coord + RIGHT;
	if(isCoordInBoard(newcoord))
		coords.push_back(newcoord);

	return coords;
}


void Board::updateStorm(uint currentRound)
{
	stormwidth = Entity::stormWidthForRound(currentRound);
}

std::vector<Player *> *Board::getPlayer() const
{
	return players;
}
