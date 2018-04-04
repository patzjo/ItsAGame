#pragma once

#include "GameObject.h"
#include "QTree.h"

class CollisionQTree : public QTree<GameObject>
{
public:
	void add(class GameObject *object);
	void remove(class GameObject *object);
	void updateTree();
	void draw(class Renderer *renderer, sf::Color color, float borderSize);

private:
	std::vector <class GameObject *> trackedObjects;
};

