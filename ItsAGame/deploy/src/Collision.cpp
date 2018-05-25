#include "Renderer.h"
#include "Collision.h"


void CollisionQTree::add(class GameObject *object)
{
	trackedObjects.push_back(object);
	insert(object, object->position);
}

void CollisionQTree::remove(GameObject * object)
{
	for (auto itr = trackedObjects.begin(); itr != trackedObjects.end(); itr++)
	{
		if (*itr == object)
		{
			trackedObjects.erase(itr);
			return;
		}
	}
}

void CollisionQTree::updateTree()
{
	clear();
	for (auto object : trackedObjects)
	{
		insert(object, object->position);
	}
}

void CollisionQTree::draw(class Renderer *renderer, sf::Color color, float borderSize)
{
	if (!showDebugRectangles)
		return;

	for (int c = 0; c < indexCount; c++)
		nodes[c].draw(renderer->getWindow(), color, borderSize);

	if (showDebugRectangles >= 2)
	for (int c = 0; c < indexCount; c++)
	{
		int size = 8;

		size += 4 * (int)nodes[c].objects.size();
		renderer->pushText(std::to_string(nodes[c].objects.size()), nodes[c].cell.centerPos, 0, size, sf::Color::Yellow, true);
	}
}

void CollisionQTree::pushLevelCollisionRectangle(Rectangle<float> rect)
{
	levelCollisionBoxes.push_back(rect);
}

void CollisionQTree::setLevelCollisionBoxes(std::vector<Rectangle<float>> LCB)
{
	levelCollisionBoxes = LCB;
}

