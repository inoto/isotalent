// IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER (AND ALSO FINISH CELL)
#include "Pathfinder.h"

Pathfinder::Pathfinder(Map* map)
{
	this->map = map;
	return;
}
Pathfinder::~Pathfinder()
{
	return;
}

int Pathfinder::computeH(Cell* cell)
{
	int toGoal = abs(goalCell->i - cell->i) + abs(goalCell->j - cell->j);
	//int toGoal = pow( (m_goalcell->j - cell->j), 2) + pow( (m_goalcell->i - cell->i),2);
	//toGoal = sqrt(toGoal);
	cell->heuristic = cell->cost + toGoal;
	return cell->heuristic;
}

void Pathfinder::checkConnected(Cell* cell)
{
	checkCell(cell->i, cell->j - 1, cell->cost + 1, cell);
	//down
	checkCell(cell->i, cell->j + 1, cell->cost + 1, cell);
	//left
	checkCell(cell->i - 1, cell->j, cell->cost + 1, cell);
	//right
	checkCell(cell->i + 1, cell->j, cell->cost + 1, cell);
	return;
}

void Pathfinder::checkCell(int i, int j, int cost, Cell* parent)
{
	//out of bounds
	if (i < 0 || i > size - 1 || j < 0 || j > size - 1)
		return;
	//Get cell at the position in the grid
	Cell* cell = &(map->cell[i][j]);
	// is cell blocked by user?
	if (cell->isBlocked)
		return;
	//Set cost if not done yet
	if (cell->cost == 0)
		cell->cost = cost;

	//Find in OPEN & CLOSE
	bool inOpen = betterExistInList(open, cell);
	bool inClosed = betterExistInList(closed, cell);
	//if not in any list or no better exists
	if (!inOpen && !inClosed)
	{
		cell->parent = parent;
		computeH(cell);
		printf("[OPEN] Cell(%d,%d) added (h:%d)\n", i, j, cell->heuristic);
		open.push_back(cell);
	}
	return;
}

void Pathfinder::buildBestPath()
{
	if (procCell->equals(startCell))
		return;
	// mark best cells
	procCell->finalWayID = procCell->cost;
	Cell* tmpCell = procCell;
	// go best way like through a list
	procCell = procCell->parent;
	procCell->son = tmpCell;
	return;
}

Cell* Pathfinder::popBest()
{
	int bestIndex = 0;
	int bestHeuristic = 50000;
	int bestCost = 50000;
	Cell* tempCell;
	//find best heuristic index
	for (int i = 0; i < open.size(); i++)
	{
		tempCell = open.at(i);
		/*if ((tempcell->heuristic < bestHeuristic)
		|| (tempcell->heuristic == bestHeuristic && tempcell->cost < bestCost) ) {
		bestIndex = i;
		bestHeuristic = tempcell->heuristic;
		bestCost = tempcell->cost;
		}*/
		if (tempCell->heuristic < bestHeuristic)
		{
			bestIndex = i;
			bestHeuristic = tempCell->heuristic;
			bestCost = tempCell->cost;
		}
	}
	//get best cell
	Cell* cell = &(*open.at(bestIndex));
	//erase it from list
	open.erase(open.begin() + bestIndex);
	return cell;
}

int Pathfinder::findIndexInList(std::vector<Cell*> list, Cell* toFind)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (toFind->equals(list.at(i)))
		{
			return i;
		}
	}
	return -1;
}

bool Pathfinder::betterExistInList(std::vector<Cell*> list, Cell* toFind)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (toFind->equals(list.at(i)))
		{
			if (list.at(i)->cost <= toFind->cost)
				return true;
		}
	}
	return false;
}

void Pathfinder::start()
{
	started = true;
	startCell = map->playerCell;
	goalCell = map->playerFutureCell;

	//First cell to check
	open.push_back(startCell);
	return;
}
void Pathfinder::update()
{
	// if we achieve target cell then do choose best way
	if (found)
	{
		buildBestPath();
		return;
	}
	if (!started || open.size() <= 0)
		return;
	procCell = popBest();
	procCell->visited = true;
	int i = procCell->i, j = procCell->j;
	if (procCell->equals(goalCell))
	{
		found = true;
		return;
	}
	else
		checkConnected(procCell);
	closed.push_back(procCell);
	return;
}

void Pathfinder::clean()
{
	if (open.size() > 0)
		open.clear();
	if (closed.size() > 0)
		closed.clear();
	startCell = nullptr;
	goalCell = nullptr;
	procCell = nullptr;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			map->cell[i][j].son = nullptr;
			map->cell[i][j].heuristic = 0;
			map->cell[i][j].cost = 0;
			map->cell[i][j].visited = false;
			map->cell[i][j].finalWayID = -1;
		}
	}
	started = false;
	found = false;
	return;
}
