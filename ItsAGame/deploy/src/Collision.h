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

	void pushLevelCollisionRectangle(Rectangle<float> rect);
	const std::vector <Rectangle<float>>& getLevelCollisionBoxes() { return levelCollisionBoxes; }
	void setLevelCollisionBoxes(std::vector <Rectangle<float>>LCB);

private:
	std::vector <class GameObject *> trackedObjects;
	std::vector <Rectangle<float>> levelCollisionBoxes;

	class Level *level;
};

