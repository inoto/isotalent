// IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER (AND ALSO FINISH CELL)
#include "Map.h"

Cell::Cell()
{
	if (!this->texture.loadFromFile("resources/sp.png"))
		printf("Error texturing...\n");
	return;
}

Cell::~Cell()
{
	return;
}

void Cell::draw(sf::RenderWindow& window)
{
	window.draw(this->sprite);
	return;
}

Cannon::Cannon()
{
	if (!this->texLeft.loadFromFile("resources/can_left.png"))
		printf("Error texturing...\n");
	if (!this->texRight.loadFromFile("resources/can_right.png"))
		printf("Error texturing...\n");
	return;
}

Cannon::~Cannon()
{

}

void Cannon::draw(sf::RenderWindow& window)
{
	window.draw(this->sprite);
	return;
}

Fireball::Fireball()
{
	return;
}

Fireball::~Fireball()
{
	return;
}

void Fireball::draw(sf::RenderWindow& window)
{
	window.draw(this->sprite);
	return;
}

Keeper::Keeper()
{
	if (!this->texture.loadFromFile("resources/owl.png"))
		printf("Error texturing...\n");
	return;
}

Keeper::~Keeper()
{
	currentCell = nullptr;
	futureCell = nullptr;
	return;
}

void Keeper::draw(sf::RenderWindow& window)
{
	window.draw(this->sprite);
	return;
}

Map::Map()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cell[i][j].i = i;
			cell[i][j].j = j;
		}
	}
	timerFireballLeft = 0;
	timerFireballRight = 0;
	timerKeeper = 0;
	zoomPos = 0;
	isPlayerOnStart = true;
	gameover = false;
	inPlayerMove = false;
	playerCell = nullptr;
	playerFutureCell = nullptr;

	if (!this->texturePlayer.loadFromFile("resources/cat.png"))
		printf("Error texturing...\n");
	if (!this->texFireballLeft.loadFromFile("resources/fb_left.png"))
		printf("Error texturing...\n");
	if (!this->texFireballRight.loadFromFile("resources/fb_right.png"))
		printf("Error texturing...\n");
	return;
}


Map::~Map()
{
	playerCell = nullptr;
	playerFutureCell = nullptr;
	fireballsLeft.clear();
	fireballsRight.clear();
	return;
}

Cell* Map::selectCell(sf::Vector2f posSelect)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			sf::FloatRect fr = cell[i][j].sprite.getGlobalBounds();
			// for debugging
			int h1x = fr.left + fr.width - fr.width / 4;
			int h1y = fr.left + fr.width / 4;
			int h2x = fr.top + fr.height - fr.height / 4;
			int h2y = fr.top + fr.height / 4;
			if (((posSelect.x <= (h1x)) && (posSelect.x >= h1y))
				&& ((posSelect.y <= (h2x)) && (posSelect.y >= h2y)))
			{
				return &cell[i][j];
			}
		}
	}
	return nullptr;
}

sf::Vector2i Map::getCellNumber(sf::Vector2f posSelect)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			sf::FloatRect fr = cell[i][j].sprite.getGlobalBounds();
			if (((posSelect.x <= (fr.left + fr.width - fr.width / 4)) && (posSelect.x >= fr.left + fr.width / 4))
				&& ((posSelect.y <= (fr.top + fr.height - fr.height / 4)) && (posSelect.y >= fr.top + fr.height / 4)))
				return{i, j};
		}
	}
	return {0,0};
}

void Map::clockUpdate()
{
	if (clock.getElapsedTime().asSeconds() >= 1.f)
	{
		fps = frame;
		frame = 0;
		clock.restart();
	}
	++frame;
	return;
}

void Map::movePlayerToCell(Cell* cell)
{
	if (!cell)
		return;
	if (isPlayerOnStart)
		isPlayerOnStart = false;
	sf::Vector2f pos(cell->sprite.getPosition());
	sprite.setPosition({ pos.x - 22, pos.y - 22 });
	playerCell = cell;
	// check that player is on finish cell
	if (playerCell == &(this->cell[0][0]))
		gameover = true;
	// if new destination setting up then stop moving for farther clean and start pathfinder
	if (playerCell == playerFutureCell)
		inPlayerMove = false;
	return;
}

// this draw is NOT optimized because of had no time.
// some peaces could be stored into separate methods etc.
void Map::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < size; ++i)
	{
		// check for wall
		cell[i][0].hasCannon = true;
		for (int j = 0; j < size; ++j)
		{
			// check for wall
			cell[0][j].hasCannon = true;
			cell[i][j].sprite.setTexture(cell[i][j].texture);
			// 
			sf::Vector2f pos({ (float)((i - j)*(texSize - 15 + 3) + indentX),(float)((i + j)*(texSize - 15 + 3) + indentY) });
			cell[i][j].sprite.setPosition(pos.x, pos.y);
			if ((i == size - 1) && (j == size - 1) && isPlayerOnStart)
				this->playerCell = &cell[i][j];
			// rotation cells 45 degrees for isometric
			cell[i][j].sprite.setRotation(45.f);
			// change the color of selected cell for best understanding
			if (cell[i][j].isBlocked)
				cell[i][j].sprite.setColor(sf::Color::Red);
			else if (&cell[i][j] == playerFutureCell)
				cell[i][j].sprite.setColor(sf::Color::Blue);
			// was for debug
			//else if (cell[i][j].visited == true)
			//	cell[i][j].sprite.setColor(sf::Color::Yellow);
			else
				cell[i][j].sprite.setColor(sf::Color::White);
			if (cell[i][j].finalWayID >= 0)
				cell[i][j].sprite.setColor(sf::Color::Green);

			cell[i][j].draw(window);

			// draw cannons with public loop with cells
			if (cell[i][j].hasCannon && i == 0)
			{
				cannonLeft[j].sprite.setTexture(cannonLeft[j].texLeft);
				sf::Vector2f pos = cell[i][j].sprite.getPosition();
				cannonLeft[j].sprite.setPosition(pos.x - texSize + 9, pos.y - texSize - texSize / 2 - 9);
				cannonLeft[j].sprite.setScale(0.7f, 1.7f);
				cannonLeft[j].sprite.setColor(sf::Color::White);
				cannonLeft[j].draw(window);
			}
			if (cell[i][j].hasCannon && j == 0)
			{
				cannonRight[i].sprite.setTexture(cannonRight[i].texRight);
				sf::Vector2f pos = cell[i][j].sprite.getPosition();
				cannonRight[i].sprite.setPosition(pos.x + 15/2-7, pos.y - texSize - texSize/3-17);
				cannonRight[i].sprite.setScale(0.7f, 1.7f);
				cannonRight[i].sprite.setColor(sf::Color::White);
				cannonRight[i].draw(window);
			}
		}
	}
	// once per second check for cannons
	if (frame == 1)
	{
		if (timerFireballLeft < 4)
			++timerFireballLeft;
		else
		{
			// random cannon makes a fireball. I know that stupid to let cannon makes a fireball on player's start position since 4 secs
			// TODO: remove zeroed left cannon for good beginning
			int divider = 15;
			for (int i = 1; i <= size / divider; ++i)
			{
				// devide wall into sections in dependence of map size
				srand(clock.getElapsedTime().asMicroseconds());
				int r = (rand() % (i*divider) - (i*divider-divider)) + (i*divider - divider);
				Fireball fireball;
				sf::Vector2f pos(cell[0][r].sprite.getPosition().x - 60, cell[0][r].sprite.getPosition().y - 60);
				fireball.sprite.setPosition(pos.x, pos.y);
				fireball.sprite.setScale(1.0f, 2.0f);
				fireball.draw(window);
				fireballsLeft.push_back(fireball);
				fireballsLeft.back().sprite.setTexture(texFireballLeft);
			}
			
			timerFireballLeft = 0;
		}
		if (timerFireballRight < 3)
			++timerFireballRight;
		else
		{
			int divider = 15;
			for (int i = 1; i <= size / divider; ++i)
			{
				srand(clock.getElapsedTime().asMicroseconds());
				int r = (rand() % (i*divider) - (i*divider - divider)) + (i*divider - divider);
				Fireball fireball;
				sf::Vector2f pos = cell[r][0].sprite.getPosition();
				fireball.sprite.setPosition(pos.x + 60, pos.y - 60);
				fireball.sprite.setScale(1.0f, 2.0f);
				fireball.draw(window);
				fireballsRight.push_back(fireball);
				fireballsRight.back().sprite.setTexture(texFireballRight);
			}
			timerFireballRight = 0;
		}
	}

	// once per second too but that is for keeper
	if (frame == 1 && !keeper.inMove)
	{
		// once per 7 secs timer
		if (timerKeeper < 7)
			++timerKeeper;
		else
		{
			if (keeper.isOnStart)
			{
				srand(clock.getElapsedTime().asMicroseconds());
				int d = (1 + rand() % ((size - 1 - size / 3) - size / 3) + size / 3);
				srand(clock.getElapsedTime().asMicroseconds());
				int b = (1 + rand() % ((size - 1 - size / 3) - size / 3) + size / 3);
				keeper.currentCell = &cell[d][b];
				keeper.ci = d;
				keeper.cj = b;
				sf::Vector2f pos = cell[d][b].sprite.getPosition();
				keeper.sprite.setTexture(keeper.texture);
				keeper.sprite.setPosition(pos.x - 22, pos.y - 22);
				keeper.sprite.setScale(1.0f, 2.0f);
				keeper.isOnStart = false;
			}
			srand(clock.getElapsedTime().asMicroseconds());
			int direction = (1 + rand() % 2);
			srand(clock.getElapsedTime().asMicroseconds());
			int b = (1 + rand() % (size - 1));
			sf::Vector2f posNum = keeper.sprite.getPosition();
			sf::Vector2i nums = getCellNumber({ posNum.x + 22,posNum.y + 45 });
			// 2 directions and random cell in the region of map
			if (direction == 1)
				keeper.futureCell = &cell[b][nums.y];
			else
				keeper.futureCell = &cell[nums.x][b];
			// set up line and circle for keeper's path
			sf::Vector2f posLine = keeper.currentCell->sprite.getPosition();
			keeper.path[0] = sf::Vertex({posLine.x, posLine.y + 32 });
			keeper.path[0].color = sf::Color::Blue;
			sf::Vector2f posFut = keeper.futureCell->sprite.getPosition();
			keeper.path[1] = sf::Vertex({ posFut.x, posFut.y + 32 });
			keeper.path[1].color = sf::Color::Blue;
			keeper.circle.setRadius(4);
			keeper.circle.setPosition({ keeper.path[1].position.x - 6, keeper.path[1].position.y - 6});
			keeper.circle.setFillColor(sf::Color::Blue);
			keeper.inMove = true;
			timerKeeper = 0;
		}
		keeper.draw(window);
	}
	if (keeper.inMove)
	{
		// TODO: stack variables names are discaster
		sf::Vector2f pos = keeper.sprite.getPosition();
		sf::Vector2f posFuture = keeper.futureCell->sprite.getPosition();
		if (posFuture.x >= pos.x + 22 && posFuture.y >= pos.y + 22)
			keeper.sprite.setPosition(pos.x + 1, pos.y + 1);
		else if (posFuture.x <= pos.x + 22 && posFuture.y >= pos.y + 22)
			keeper.sprite.setPosition(pos.x - 1, pos.y + 1);
		else if (posFuture.x <= pos.x + 22 && posFuture.y <= pos.y + 22)
			keeper.sprite.setPosition(pos.x - 1, pos.y - 1);
		else if (posFuture.x >= pos.x + 22 && posFuture.y <= pos.y + 22)
			keeper.sprite.setPosition(pos.x + 1, pos.y - 1);
		sf::Vector2f cellPos = keeper.sprite.getPosition();
		Cell* cell = selectCell({ cellPos.x + 22,cellPos.y + 45});
		if (cell == playerCell)
			//printf("GAVE OVER\n");
			gameover = true;
		keeper.currentCell = cell;
		if (keeper.currentCell == keeper.futureCell)
		{
			sf::Vector2f pos = keeper.futureCell->sprite.getPosition();
			keeper.sprite.setPosition(pos.x - 22, pos.y - 22);
			keeper.inMove = false;
		}
		// line and circle for keeper's path
		window.draw(keeper.path, 2, sf::Lines);
		window.draw(keeper.circle);
	}
	// keeper could catch you!
	if (playerCell == keeper.currentCell)
		//printf("GAVE OVER\n");
		gameover = true;
	keeper.draw(window);
	// draw and move every exist fireball (left side)
	for (std::vector<Fireball>::iterator it = fireballsLeft.begin(); it != fireballsLeft.end(); ++it)
	{
		sf::Vector2f pos = it->sprite.getPosition();
		it->sprite.setPosition(pos.x + 2, pos.y + 2);
		++(it->timerFlight);
		//printf("timerFlight = %d\n",it->timerFlight);
		it->draw(window);
		//define a cell where fireball right now and compare with player's cell
		if (it->timerFlight == 22)
		{
			printf("%d timerFlight = %d\n", it,it->timerFlight);
			it->timerFlight == 0;
			Cell* cell = selectCell(pos);
			if (playerCell == cell)
				//printf("GAVE OVER\n");
				gameover = true;
		}
		// remove fireballs when they are not so dangerous
		if (pos.x > 1024 && pos.y > 768)
		{
			fireballsLeft.erase(it);
			break;
		}
	}
	// draw and move every exist fireball (right side)
	for (std::vector<Fireball>::iterator it = fireballsRight.begin(); it != fireballsRight.end(); ++it)
	{
		sf::Vector2f pos = it->sprite.getPosition();
		it->sprite.setPosition(pos.x - 2, pos.y + 2);
		it->draw(window);
		//define a cell where fireball right now and compare with player's cell
		if (it->timerFlight == texSize / 2)
		{
			it->timerFlight == 0;
			Cell* cell = selectCell(pos);
			if (playerCell == cell)
				//printf("GAVE OVER\n");
				gameover = true;
		}
		// remove fireballs when they are not so dangerous
		if (pos.x < -1024 && pos.y > 768)
		{
			fireballsRight.erase(it);
			break;
		}
	}
	// draw player
	sf::Vector2f pos(playerCell->sprite.getPosition());
	sprite.setPosition({ pos.x - 22, pos.y - 22 });
	sprite.setTexture(texturePlayer);
	sprite.setScale(1.0f, 2.0f);
	window.draw(sprite);
	return;
}
