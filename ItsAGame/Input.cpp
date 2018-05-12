#include <memory>
#include <vector>
#include "Command.h"
#include "Input.h"
#include "GameObject.h"
#include "Game.h"

Input::Input()
{
	availableCommands.push_back({ "Move Left", new MoveLeft });
	availableCommands.push_back({ "Move Right", new MoveRight });
	availableCommands.push_back({ "Fire Cannon", new FireCommand });
	availableCommands.push_back({ "Move Cannon Counterclockwise", new IncreaseCannonAngle });
	availableCommands.push_back({ "Move Cannon Clockwise", new DecreaseCannonAngle });
}

Input::~Input()
{
}

void Input::processInput(Game *game, Player * playerObject)
{
	if (!playerObject) return;
	for (auto& input : playerObject->getInputComponent()->commands)
	{
		if (sf::Keyboard::isKeyPressed(input.key))
			input.command->execute(game, playerObject);
	}
}

void Input::setKeysFromOptions(class Game *game)
{
	for (unsigned int c = 0; c < game->getPlayerCount(); c++)
	{
		if (game->getPlayer(c))
		{
			for (unsigned int i = 0; i < 5; i++)
			{
				std::cout << "Pushin key: " << game->getOptions()->playerKeys[c][i].key << " -> player[" << c << "] = " << game->getOptions()->playerKeys[c][i].name << std::endl;
				game->getPlayer(c)->getInputComponent()->pushCommand(
					game->getOptions()->playerKeys[c][i].key, 
					getCommand(game->getOptions()->playerKeys[c][i].name)
				);
			}
		}
	}
}

Command * Input::getCommand(std::string name)
{
	for (auto& command : availableCommands)
	{
		if (command.name == name)
		{
			return command.command;
		}
	}
	return nullptr;
}

