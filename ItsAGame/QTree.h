#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Utils.h"

template <typename T>
struct QTreeNodeWrapper
{
	sf::Vector2f position;
	T *object;
};

template <typename T>
struct QTreeNode
{
	Square <float>cell;
	bool inUse = false;

	QTreeNode <T> *topLeft		= nullptr;
	QTreeNode <T> *topRight		= nullptr;
	QTreeNode <T> *bottomLeft	= nullptr;
	QTreeNode <T> *bottomRight	= nullptr;

	std::vector <QTreeNodeWrapper<T>> objects;

	void drawWithChildren(sf::RenderWindow& w, sf::Color color, float borderSize)
	{
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(color);
		rect.setOutlineThickness(borderSize);

		rect.setPosition(cell.centerPos.x - cell.halfSize.x, cell.centerPos.y - cell.halfSize.y);
		rect.setSize(cell.halfSize * 2);

		w.draw(rect);

		if (topLeft)
			topLeft->draw(w, color, borderSize);
		if (topRight)
			topRight->draw(w, color, borderSize);

		if (bottomLeft)
			bottomLeft->draw(w, color, borderSize);
		if (bottomRight)
			bottomRight->draw(w, color, borderSize);
	}

	void draw(sf::RenderWindow& w, sf::Color color, float borderSize)
	{
		if (!inUse)
			return;
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(color);
		rect.setOutlineThickness(borderSize);

		rect.setPosition(cell.centerPos.x - cell.halfSize.x, cell.centerPos.y - cell.halfSize.y);
		rect.setSize(cell.halfSize * 2);

		w.draw(rect);
	}

	std::vector <T *> getObjectsInRange(Square<float> area)
	{
		std::vector <T *> objectList;

		if (!area.boxIntersect(cell))
			return objectList;
		
		for (QTreeNodeWrapper<T>& object : objects)
		{
			if (area.containsPoint(object.position))
				objectList.push_back(object.object);
		}

		if (topLeft == nullptr)
			return objectList;

		if (topLeft->inUse)
		{
			auto temp = topLeft->getObjectsInRange(area);
			objectList.insert(std::end(objectList), std::begin(temp), std::end(temp));
		}
		if (topRight->inUse)
		{
			auto temp = topRight->getObjectsInRange(area);
			objectList.insert(std::end(objectList), std::begin(temp), std::end(temp));
		}
		if (bottomLeft->inUse)
		{
			auto temp = bottomLeft->getObjectsInRange(area);
			objectList.insert(std::end(objectList), std::begin(temp), std::end(temp));
		}
		if (bottomRight->inUse)
		{
			auto temp = bottomRight->getObjectsInRange(area);
			objectList.insert(std::end(objectList), std::begin(temp), std::end(temp));
		}

		return objectList;
	}

	QTreeNode() {};
};

template <typename T>
class QTree
{
public:
	QTree() {
		//initialize(3, 4, Square<float>({ 960.0f, 540.0f }, { 1920.0f/2.0f, 1080.0f/2.0f }));
	}
	
	bool initialize(int MaxDepth, int NodeCapacity, Square<float> area)
	{
		if (nodes)
			delete[] nodes;

		MAX_DEPTH = MaxDepth;
		NODE_CAPACITY = NodeCapacity;
		

		indexCount = 2; // First one is empty
		for (int c = 1; c <= MaxDepth; c++)
		{
			indexCount += (int)pow(4.0f, (float)c);
		}


		std::cout << "Init: " << indexCount << std::endl;
		nodes = new QTreeNode <T>[indexCount];

		if (nodes)
		{
			nodes[0].cell = area;
			calculateBoundaries(&nodes[0], 0);
			clear();
		}
		else
			indexCount = 0;

		return nodes;
	}
	
	void calculateBoundaries(QTreeNode <T> *node, int index)
	{
		if ((index * 4) + 4 >= indexCount)
		{
			node->topLeft = nullptr;
			node->topRight = nullptr;
			node->bottomLeft = nullptr;
			node->bottomRight = nullptr;
			return;
		}

		Square<float> sq[4];
		
		float childrenHalfSizeX = node->cell.halfSize.x / 2.0f;
		float childrenHalfSizeY = node->cell.halfSize.y / 2.0f;

		sq[0].centerPos.x = node->cell.centerPos.x - childrenHalfSizeX;
		sq[0].centerPos.y = node->cell.centerPos.y - childrenHalfSizeY;

		sq[1].centerPos.x = node->cell.centerPos.x + childrenHalfSizeX;
		sq[1].centerPos.y = node->cell.centerPos.y - childrenHalfSizeY;
		
		sq[2].centerPos.x = node->cell.centerPos.x - childrenHalfSizeX;
		sq[2].centerPos.y = node->cell.centerPos.y + childrenHalfSizeY;
		
		sq[3].centerPos.x = node->cell.centerPos.x + childrenHalfSizeX;
		sq[3].centerPos.y = node->cell.centerPos.y + childrenHalfSizeY;

		sf::Vector2f halfSize = node->cell.halfSize * 0.5f;
		
		sq[0].halfSize = halfSize;
		sq[1].halfSize = halfSize;
		sq[2].halfSize = halfSize;
		sq[3].halfSize = halfSize;

		int i0 = (index * 4) + 1;		// First child
		int i1 = (index * 4) + 2;		// Second child
		int i2 = (index * 4) + 3;		// Third child
		int i3 = (index * 4) + 4;		// Fourth child


		node->topLeft     = &nodes[i0];
		node->topRight    = &nodes[i1];
		node->bottomLeft  = &nodes[i2];
		node->bottomRight = &nodes[i3];

		node->topLeft->cell		= sq[0];
		node->topRight->cell	= sq[1];
		node->bottomLeft->cell	= sq[2];
		node->bottomRight->cell	= sq[3];

		calculateBoundaries(node->topLeft,		i0);
		calculateBoundaries(node->topRight,		i1);
		calculateBoundaries(node->bottomLeft,	i2);
		calculateBoundaries(node->bottomRight,	i3);
	}

	void clear()
	{
		if (nodes == nullptr)
			return;

		for (int c = 0; c < indexCount; c++)
		{
			nodes[c].inUse			= false;
			nodes[c].objects.clear();
		}
	}

	bool _insert(QTreeNode<T> *node, QTreeNodeWrapper <T>item)
	{
		
		if (node->cell.containsPoint(item.position))
		{
			if (node->objects.size() < NODE_CAPACITY)
			{
				node->objects.push_back(item);
				node->inUse = true;
				return true;
			}
			else
			{
				bool top = true;
				bool left = true;

				if (item.position.x > node->cell.centerPos.x)
					left = false;
				if (item.position.y > node->cell.centerPos.y)
					top = false;
				 
				if (node->topLeft == nullptr)
					return false;
				
				bool success = false;

				if (top  &&  left)   success = _insert(node->topLeft, item);
				if (top  && !left)   success = _insert(node->topRight, item);
				if (!top &&  left)   success = _insert(node->bottomLeft, item);
				if (!top && !left)   success = _insert(node->bottomRight, item);

				return success;
			}
		}
		else
			return false;
	}
			
	void insert(T *item, sf::Vector2f position)
	{
		QTreeNodeWrapper<T> object;
		object.position = position;
		object.object = item;
		
		_insert(&nodes[0], object);
		
	}
	std::vector <T *> getObjectsInRange(Square<float> area)
	{
		return nodes[0].getObjectsInRange(area);
	}

	~QTree()
	{
		if (nodes)
			delete[] nodes;
	}

	int getIndexCount() { return indexCount; }
	QTreeNode <T> *getNodes() { return nodes; }


	void toggleDebug() { 
		showDebugRectangles++;
		if (showDebugRectangles > 2)
			showDebugRectangles = 0;
	}

//protected:
	QTreeNode <T> *nodes = nullptr;
	int NODE_CAPACITY = 4;
	int MAX_DEPTH = 0;

	int indexCount = 0;

	int showDebugRectangles = false;
		
};