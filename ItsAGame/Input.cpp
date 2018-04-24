#include <memory>
#include <vector>
#include "Input.h"
#include "Command.h"
#include "GameObject.h"




void Input::processInput(Game *game, Player * playerObject)
{
	std::vector <Command *> commands;
	
	FireCommand fire;
	IncreaseCannonAngle angleInc;
	DecreaseCannonAngle angleDec;
	MoveLeft moveLeft;
	MoveRight moveRight;


	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		commands.push_back(new MoveLeft);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		commands.push_back(new MoveRight);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		commands.push_back(new FireCommand);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		commands.push_back(new IncreaseCannonAngle);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		commands.push_back(new DecreaseCannonAngle);

	for (auto& command : commands)
	{
		command->execute(game, playerObject);
		delete command;
	}
}
