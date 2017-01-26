// IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER (AND ALSO FINISH CELL)
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <list>

// the map is scalable but there are lags because of my moody find algoryphms
const int size = 30;
const int texSize = 45;
const int indentX = 100;
const int indentY = 100;

const float zoomAmount = 1.2f;

class Cell : sf::Sprite
{
	// TODO: hide some parameters and methods
public:
	Cell();
	virtual ~Cell();

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;

	bool hasCannon = false;
	bool isBlocked = false;
	bool visited = false;

	int heuristic = 0;
	int i = 0, j = 0;
	int cost = 0;
	int finalWayID = -1;

	Cell* parent = nullptr;
	Cell* son = nullptr;

	bool equals(Cell * c)
	{
		return c->i == i && c->j == j;
	}

	void draw(sf::RenderWindow&);
};

class Cannon : sf::Sprite
{
	// TODO: hide some parameters and methods
public:
	Cannon();
	virtual ~Cannon();

	sf::Sprite sprite;
	sf::Texture texLeft;
	sf::Texture texRight;
	sf::Image image;

	void draw(sf::RenderWindow&);
};

class Fireball : sf::Sprite
{
	// TODO: hide some parameters and methods
public:
	Fireball();
	virtual ~Fireball();

	sf::Sprite sprite;
	sf::Image image;

	int timerFlight = 0;
	int ci = 0, cj = 0;

	void draw(sf::RenderWindow&);
};

class Keeper : sf::Sprite
{
	// TODO: hide some parameters and methods
public:
	Keeper();
	virtual ~Keeper();

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;
	sf::Vertex path[2];
	sf::CircleShape circle;

	bool inMove = false;
	bool isOnStart = true;

	int ci = 0, cj = 0;

	Cell* currentCell = nullptr;
	Cell* futureCell = nullptr;

	void draw(sf::RenderWindow&);

};

class Map
{
	// TODO: hide some parameters and methods
public:
	Map();
	~Map();

	sf::Clock clock;
	unsigned int frame = 0;
	unsigned int fps = 0;
	void clockUpdate();

	Cell cell[size][size];

	Cannon cannonLeft[size];
	Cannon cannonRight[size];

	Keeper keeper;

	sf::Sprite sprite;
	sf::Texture texturePlayer;
	sf::Texture texFireballLeft;
	sf::Texture texFireballRight;
	sf::Image image;

	// TODO: need investigation about const
	Cell* playerCell = nullptr;
	Cell* playerFutureCell = nullptr;

	std::vector<Fireball> fireballsLeft;
	std::vector<Fireball> fireballsRight;

	bool isPlayerOnStart = true;
	bool gameover = false;
	bool inPlayerMove = false;

	int zoomPos = 0;
	int timerFireballLeft = 0;
	int timerFireballRight = 0;
	int timerKeeper = 0;
	int end = 0;
	int ci = 0, cj = 0;
	
	Cell* selectCell(sf::Vector2f);
	sf::Vector2i getCellNumber(sf::Vector2f);
	void movePlayerToCell(Cell*);

	void draw(sf::RenderWindow&);
};

