#include "Renderer.h"
#include "Collision.h"


void CollisionQTree::add(class GameObject *object)
{
	trackedObjects.push_back(object);
	insert(object, object->position);
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

		size += 4 * nodes[c].objects.size();
		renderer->pushText(std::to_string(nodes[c].objects.size()), nodes[c].cell.centerPos, 0, size, sf::Color::Yellow, true);
	}
}

