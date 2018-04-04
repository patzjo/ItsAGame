#pragma once
#include <SFML/Graphics.hpp>

#include "Utils.h"

class Level
{
public:
	Level();
	~Level();
	void adjust(std::vector<int>& vec, int width, int height, int minWidth, int maxWidth);
//	void generateRectangleLevel(int width, int height, int minHeight, int maxHeight, int minWidth, int maxWidth);
	void generateRectangleLevel(int width, int height, int minHeight, int maxHeight, int minWidth, int maxWidth, sf::Color fillColor = sf::Color::White, sf::Color outlineColor = sf::Color::White);

	std::vector <int> temp;
	sf::Texture& getTexture() { return levelTexture; }
	void rendererConfirm()    { rendererNeedUpdate = false; };
	bool needUpdate()		  { return rendererNeedUpdate; }

	void doCircleHole(sf::Vector2f pos, float radius, sf::Color color, bool centered = true);


private:
	sf::Image levelData;
	sf::Texture levelTexture;
	bool rendererNeedUpdate = true;

	std::vector <Square<float>> hitBoxes;
};

