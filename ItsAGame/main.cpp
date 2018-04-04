#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"

using std::endl;
using std::cout;



int main(int argc, char **argv)
{
	Game game;

	game.initialize(1920, 1080, "ItsAGame!");
	game.run();

	return 0;
}
/*
int main2(int argc, char**argv)
{
	QTree <sf::Vector2f> myTree;
	sf::RenderWindow window(sf::VideoMode(1920,1080), "Hello!");
	sf::Font font;
	
	sf::Sprite levelSprite;

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::Blue);

	Level level;

	level.generateRectangleLevel(1920, 1080, 100, 500, 100, 200, sf::Color::White);
	sf::RectangleShape line;
	line.setFillColor(sf::Color::White);

	levelSprite.setTexture(level.getTexture());

	sf::Clock clock;
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				
				case sf::Keyboard::R:
					level.generateRectangleLevel(1920, 1080, 100, 500, 100, 200);
					break;

				case sf::Keyboard::A:
					break;

				default:
					break;
				}
			}
		}

		window.clear();
		window.draw(levelSprite);

		float lineStart = 0.0f;
		float lineEnd = 0.0f;
		float lineLength = 0.0f;

		for (unsigned int c = 0; c < level.temp.size(); c++)
		{
			if (c % 2 == 0)
				line.setFillColor(sf::Color::Red);
			else
				line.setFillColor(sf::Color::Green);

			line.setPosition(lineLength, 540.0f);
			line.setSize({ (float)level.temp[c], 5.0f });
			window.draw(line);
			

			text.setString(std::to_string(level.temp[c]));

			float textPosX = (lineLength + (lineLength+level.temp[c])) / 2.0f;
			float textPosY = 540.0f - text.getCharacterSize() - 10.0f;

			text.setPosition({ textPosX - text.getLocalBounds().width/2.0f, textPosY });
			window.draw(text);

			lineLength += (float)level.temp[c];
		}

		window.display();
	}

	return 0;
}
*/