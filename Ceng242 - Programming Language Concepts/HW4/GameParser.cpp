#include "GameParser.h"
#include  <fstream>
#include "Berserk.h"
#include "Tracer.h"
#include "Ambusher.h"
#include "Pacifist.h"
#include "Dummy.h"
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename)
{
	std::ifstream inFile(filename);
	std::string line;

	std::pair<int, std::vector<Player *> *> parser;
	std::vector<Player *> *p = new std::vector<Player *>;

	std::getline(inFile, line);
	int b_size = std::stoi(line.substr(12));

	std::getline(inFile, line);
	int count = std::stoi(line.substr(14));

	parser.first = b_size;

	for (int i = 0; i < count; i++)
	{
		std::getline(inFile, line);
		Player *a;

		int id = std::stoi(line.substr(0, line.find(":")));

		if(id < 10)
			line = line.substr(3);
		else
			line = line.substr(4);

		std::string type = line.substr(0, line.find(":"));

		int len = type.length();

		line = line.substr(len+2);

		std::string co_x = line.substr(0, line.find(":"));

		len = co_x.length();

		line = line.substr(len + 2);

		std::string co_y = line.substr(0);

		int coord_x = std::stoi(co_x);
		int coord_y = std::stoi(co_y);

		if(type == "Berserk")
			a = new Berserk(id, coord_x, coord_y);
		else if(type == "Tracer")
			a = new Tracer(id, coord_x, coord_y);
		else if(type == "Ambusher")
			a = new Ambusher(id, coord_x, coord_y);
		else if(type == "Pacifist")
			a = new Pacifist(id, coord_x, coord_y);
		else if(type == "Dummy")
			a = new Dummy(id, coord_x, coord_y);

		p->push_back(a);

	}
	parser.second = p;
	return parser;
}
