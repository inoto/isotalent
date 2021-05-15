// IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER (AND ALSO FINISH CELL)
#pragma once
#include "Map.h"

// I took this realization of A* pathfinder from github

class Pathfinder
{
public:
	Pathfinder(Map*);
	~Pathfinder();
	
	Map* map = nullptr;
	std::vector<Cell*> open;
	std::vector<Cell*> closed;
	Cell* startCell = nullptr;
	Cell* goalCell = nullptr;
	Cell* procCell = nullptr;

	bool started = false;
	bool found = false;

	int computeH(Cell*);
	void checkConnected(Cell*);
	void checkCell(int, int, int, Cell*);
	void buildBestPath();

	Cell* popBest();
	int findIndexInList(std::vector<Cell*>, Cell*);
	bool betterExistInList(std::vector<Cell*>, Cell*);

	void start();
	void update();
	void clean();
};

