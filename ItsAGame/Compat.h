#pragma once

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(class GameObject* gameObject) = 0;
};