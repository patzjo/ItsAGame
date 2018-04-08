#include "Utils.h"
#include "Level.h"
#include <iostream>
#include <vector>
#include <random>


Level::Level()
{
}


Level::~Level()
{
}

void Level::adjust(std::vector <int>&vec, int width, int height, int minWidth, int maxWidth)
{
	int totalWidth = 0;
	for (auto& i : vec)
		totalWidth += i;

	if (totalWidth == width)
		return;

	int difference = width - totalWidth;

	for (auto&i : vec)
	{
		if (difference == 0)
			return;

		if (difference < 0) // Too long, difference is negative
		{
			if (i > minWidth)
			{
				int maxAdjustValue = i - minWidth;

				if (maxAdjustValue >= -difference)	// If we can take stuff away same amount that difference, lets take it
				{
					i += difference;
					return; // Should be done now
				}
				else
				{
					i -= maxAdjustValue;
					difference += maxAdjustValue;
				}
			}

		}
		else // Too short, difference is positive
		{
			if (i < maxWidth)
			{
				int maxAdjustValue = maxWidth - i;
				if (maxAdjustValue >= difference)
				{
					i += difference;
					return;
				}
				else
				{
					i += maxAdjustValue;
					difference -= maxAdjustValue;
				}
			}
		}
	}

}


void Level::generateRectangleLevel(int width, int height, int minHeight, int maxHeight, int minWidth, int maxWidth, sf::Color fillColor, sf::Color outlineColor)
{
	std::random_device generator;
	
	levelData.create(width, height, sf::Color::Black);

	int minPieces = (int)((float)width / (float)maxWidth + 0.5f);
	int maxPieces = (int)((float)width / (float)minWidth + 0.5f);

	int difference = maxPieces - minPieces;

	std::uniform_int_distribution<int> distribution(0, difference);
	int pieces = minPieces + distribution(generator);

	int curLength = 0;

	std::vector <int> allPiecesWidth;

	for (int c = 0; c < pieces; c++)
	{
		std::uniform_int_distribution<int> distr(minWidth, maxWidth);
		int w = distr(generator);
		allPiecesWidth.push_back(w);
		curLength += w;
	}

	if (curLength != width)
	{
		int adjustSize = width - curLength;
		//std::cout << "level needs to be adjusted: " << adjustSize << std::endl;
		adjust(allPiecesWidth, width, height, minWidth, maxWidth);
	}

	int curPosition = 0;
	for (auto& i : allPiecesWidth)
	{
		std::uniform_int_distribution<int> distr(minHeight, maxHeight);
		int h = distr(generator);

		int towerHeight = height - h;
		for (int y = height - h; y < height; y++)
		{
			for (int x = curPosition; x < curPosition + i; x++)
			{
				if ( x == curPosition || x == curPosition + i - 1 || y == towerHeight )
					levelData.setPixel(x, y, outlineColor);
				else
					levelData.setPixel(x, y, fillColor);
			}
		}
		Rectangle<float> hitBox;
		
		hitBox.halfSize.x = i / 2.0f;
		hitBox.halfSize.y = h / 2.0f;
		
		hitBox.centerPos.x = curPosition + hitBox.halfSize.x;
		hitBox.centerPos.y = towerHeight + hitBox.halfSize.y;

		hitBoxes.push_back(hitBox);

		curPosition += i;
	}

	levelTexture.loadFromImage(levelData);
	rendererNeedUpdate = true;

	for (auto& i : hitBoxes)
	{
		std::cout << "Position:" << i.centerPos << " Size: " << i.halfSize << std::endl;
	}
}

void Level::doCircleHole(sf::Vector2f pos, float radius, sf::Color color, bool centered)
{
	int diameter = (int)radius * 2;
	
	int posX = 0;
	int posY = 0; 
	
	if (centered)
	{
		posX = (int)pos.x - (int)radius;
		posY = (int)pos.y - (int)radius;
	}
	else
	{
		posX = (int)pos.x;
		posY = (int)pos.y;
	}

	for (int y = posY; y < posY + diameter; y++)
	{
		if (y < (int)levelData.getSize().y && y >= 0)
		{
			for (int x = posX; x < posX + diameter; x++)
			{
				if (x < (int)levelData.getSize().x && x >= 0)
				{
					int radiusX = posX + (int)radius - x;
					int radiusY = posY + (int)radius - y;

					float length = sqrtf((float)radiusX*(float)radiusX + (float)radiusY * (float)radiusY);

					if (length <= radius)
					{
						levelData.setPixel(x, y, color);
					}
				}
			}
		}
	}
	levelTexture.loadFromImage(levelData);
	rendererNeedUpdate = true;
}

sf::Color Level::getDataFrom(unsigned int x, unsigned int y)
{
	if (x >= levelData.getSize().x || y >= levelData.getSize().y)
		return sf::Color::Black;
	return levelData.getPixel(x, y);
}

