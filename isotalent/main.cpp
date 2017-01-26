// author: Dmitriy Tabakov
// date: 15.06.2016
// IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER(AND ALSO FINISH CELL)

#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Map.h"
#include "Pathfinder.h"

bool mouseMoveMode = false;

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	return;
}

void moveCamera(sf::RenderWindow& window)
{
	sf::View view{ window.getView() };
	auto mousePos = sf::Mouse::getPosition(window);
	view.setCenter(-800/2, 600);
	view.move(mousePos.x, mousePos.y);
	window.setView(view);
	return;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "isotalent", sf::Style::Titlebar|sf::Style::Close);
	window.setFramerateLimit(60);

	// change the view to isometric
	sf::View view = window.getDefaultView();
	view.setSize(view.getSize().x*size/8, view.getSize().y*size/4);
	view.setCenter(50, 800);
	view.zoom((1.f / zoomAmount)/2);
	window.setView(view);

	// setup a text for ingame debug info
	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf"))
		return 1;
	sf::Text text;
	text.setFont(font);
	text.setString("GAME OVER");
	text.setCharacterSize(30);
	text.setColor(sf::Color::Yellow);

	Map map;
	Pathfinder path(&map);

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Middle)
					mouseMoveMode = true;
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Middle)
					mouseMoveMode = false;
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					sf::Vector2f coord = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
					//printf("pos x: %f pos y: %f\n", coord.x, coord.y);
					Cell* cell = map.selectCell({ coord.x, coord.y });
					if (!cell)
						break;
					if (cell == map.playerCell)
						break;
					if (cell == map.playerFutureCell)
						break;
					if (!cell->isBlocked)
						cell->isBlocked = true;
					else
						cell->isBlocked = false;
					path.update();
				}
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f coord = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
					//printf("pos x: %f pos y: %f\n", coord.x, coord.y);
					// movePlayer section
					Cell* cell = map.selectCell({ coord.x, coord.y });
					if (!cell)
						break;
					if (cell == map.playerCell)
					{
						// TODO: make cat flipped horizontally just for fun
						break;
					}
					if (cell->isBlocked)
						break;
					if (cell == map.playerFutureCell)
						break;
					else
					{
						path.clean();
						map.playerFutureCell = cell;
						path.start();
					}
					map.inPlayerMove = true;
				}
				break;
			case sf::Event::MouseMoved:
				if (mouseMoveMode == true)
				{
					auto mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
					moveCamera(window);
				}
				break;
			case sf::Event::MouseWheelMoved:
				if (event.mouseWheel.delta > 0)
				{
					if (map.zoomPos < 3)
					{
						zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, (1.f / zoomAmount));
						map.zoomPos++;
					}
				}
				else
				{
					if (map.zoomPos > -2)
					{
						zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, zoomAmount);
						map.zoomPos--;
					}
				}
				break;
			}
		}
		window.clear();
		map.clockUpdate();
		map.draw(window);
		// once per second actions for pathfinder
		if ((map.frame == 1) && (map.playerCell != map.playerFutureCell) && map.inPlayerMove)
		{
			// when full path is ready we could move
			if (path.found)
			{
				Cell* cell = map.playerCell->son;
				if (cell)
				{
					cell->finalWayID = -1;
					if (map.playerCell->parent)
						map.playerCell->parent = nullptr;
					map.movePlayerToCell(cell);
				}
			}
		}
		// player is on position
		if (map.playerCell == map.playerFutureCell)
		{
			map.playerFutureCell = nullptr;
			map.inPlayerMove = false;
			path.clean();
		}
		path.update();
		if (map.gameover)
		{
			// TODO: 1. text doesn't work
			// 2. Issues of complicated arrays like vector during shutdowning the application
			// 3. IMPORTANT! APPLICATION EXITS WHEN YOU MEAT FIREBALL OR KEEPER (AND ALSO FINISH CELL)
			//if (map.frame == 1)
			//{
			//	printf("GAVE OVER\n");
			//	map.end++;
			//}
			//if (map.end > 5)
			return 0;
			//else if (map.end == 0)
			//{
			//	text.setPosition({ map.sprite.getPosition() });
			//	text.setScale({ 2.1f,2.1f });
			//}
			//window.draw(text);
		}
		
		window.display();
	}

	return 0;
}