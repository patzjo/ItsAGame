#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct AvailableCommands
{
	std::string name;
	Command *command;
};

class Input
{
public:
	Input();
	~Input();

	void processInput(class Game *game, class Player *playerObject);
	void updateMousePosition(sf::Vector2i pos) { mousePosition = pos; }
	sf::Vector2i getMousePosition() { return mousePosition; }
	
	void setKeysFromOptions(class Game *game);

private:
	Command * getCommand(std::string name);

	sf::Vector2i mousePosition;
	std::vector <AvailableCommands> availableCommands;
};