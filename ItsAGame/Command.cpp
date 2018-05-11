#include "Command.h"
#include "Game.h"


void FireCommand::execute(class Game *game, class GameObject* gameObject)
{
	((Player *)gameObject)->shoot(game->getWorld(), game->getDeltaTime());
}

void DecreaseCannonAngle::execute(Game * game, GameObject * gameObject)
{
	((Player *)gameObject)->moveCannonAngleDown(game->getDeltaTime());
}


void IncreaseCannonAngle::execute(Game * game, GameObject * gameObject)
{
	((Player *)gameObject)->moveCannonAngleUp(game->getDeltaTime());
}

void MoveLeft::execute(Game * game, GameObject * gameObject)
{
//	if ( !gameObject->falling )
		gameObject->getPhysicsComponent()->acc.x -= 90.0f;
}

void MoveRight::execute(Game * game, GameObject * gameObject)
{
//	if (!gameObject->falling)
		gameObject->getPhysicsComponent()->acc.x += 90.0f;
}
