#pragma once
#include "GameObject.h"


class Command
{
public:
	virtual ~Command() {}
	virtual void execute(class Game *game, class GameObject* gameObject) = 0;
};

class FireCommand : public Command
{
public:
	void execute(class Game *game, class GameObject* gameObject);
};

class DecreaseCannonAngle : public Command
{
public:

	void execute(class Game *game, class GameObject* gameObject);
};

class IncreaseCannonAngle : public Command
{
public:

	void execute(class Game *game, class GameObject* gameObject);
};

class MoveLeft: public Command
{
public:

	void execute(class Game *game, class GameObject* gameObject);
};

class MoveRight: public Command
{
public:

	void execute(class Game *game, class GameObject* gameObject);
};