#pragma once
#include <SFML/Graphics.hpp>

class Input
{
public:
	void processInput();
	void updateMousePosition(sf::Vector2i pos) { mousePosition = pos; }
	sf::Vector2i getMousePosition() { return mousePosition; }
private:
	sf::Vector2i mousePosition;
};