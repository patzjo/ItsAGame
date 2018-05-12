#include <memory>
#include <vector>
#include "Input.h"
#include "Command.h"
#include "GameObject.h"




void Input::processInput(Game *game, Player * playerObject)
{
	for (auto& input : playerObject->getInputComponent()->commands)
	{
		if (sf::Keyboard::isKeyPressed(input.key))
			input.command->execute(game, playerObject);
	}
}
