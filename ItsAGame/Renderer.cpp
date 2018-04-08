#include <iostream>

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Renderer.h"
#include "Level.h"
#include "World.h"

#include "Assets.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

bool Renderer::initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vsync, bool fullScreen)
{
	window.create(sf::VideoMode(ScreenWidth, ScreenHeight), name, fullScreen?sf::Style::Fullscreen:sf::Style::Default);
	window.setVerticalSyncEnabled(vsync);

	loadGraphicAssets();

	return false;
}

void Renderer::addTexture(std::string filename)
{
	sf::Texture texture;
	texture.loadFromFile(filename);
	textures.push_back(texture);
}

void Renderer::addFont(std::string filename)
{
	sf::Font font;
	font.loadFromFile(filename);

	fonts.push_back(font);
}

void Renderer::renderLevel()
{
	if (level)
	{
		if (level->needUpdate())
		{
			levelSprite.setTexture(level->getTexture());
			level->rendererConfirm();
		}

		window.draw(levelSprite);
	}
}

void Renderer::pushRenderable(struct RenderComponent * renderComponent)
{
	renderComponents.push_back(renderComponent);
}

void Renderer::removeRenderable(struct RenderComponent *renderComponent)
{
	for (auto itr = renderComponents.begin(); itr != renderComponents.end(); itr++)
	{
		if (*itr == renderComponent)
		{
			itr = renderComponents.erase(itr);
			return;
		}
	}
}

void Renderer::drawLevelCollisionBoxes()
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Red);
	for (auto& i : level->getLevelCollisionBoxes())
	{
		rect.setPosition(i.centerPos.x - i.halfSize.x, i.centerPos.y - i.halfSize.y);
		rect.setSize(i.halfSize * 2.0f);

		window.draw(rect);
	}
}

void Renderer::render()
{
	window.clear();

	renderLevel();
	renderGameObjects();

	// Drawing test object
	sf::CircleShape shape;
	shape.setRadius(10.0f);
	shape.setOrigin(10.0f, 10.0f);
	shape.setFillColor(sf::Color::Red);

	for (RenderComponent *c : renderComponents)
	{
		shape.setPosition(c->parent->position);
		window.draw(shape);
	}
		
	world->collisionTree.draw(this, sf::Color::Green, 1.0f);
//	drawLevelCollisionBoxes();
	renderTexts();
	window.display();
}

// TEXT RENDERING ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::pushText(std::string text, sf::Vector2f pos, int fontID, int characterSize, sf::Color color, bool centered)
{
	RendererTextObject object;
	object.text = text;
	object.pos = pos;
	object.fontID = fontID;
	object.characterSize = characterSize;
	object.color = color;
	object.centered = centered;

	textObjects.push_back(object);
}

void Renderer::loadGraphicAssets()
{
	for (unsigned int c = 0; c < sizeof(assets) / sizeof(assets[0]); c++)
	{
		switch (assets[c].type)
		{
		case FONT:
			addFont(assets[c].filename);
			break;
		case TEXTURE:
			addTexture(assets[c].filename);
			break;

		default: break;
		}
	}

}

void Renderer::renderTexts()
{
	sf::Text text;
	for (auto& item : textObjects)
	{
		text.setFont(fonts[item.fontID]);
		text.setString(item.text);
		text.setCharacterSize(item.characterSize);
		if (item.centered)
		{
			text.setPosition({ item.pos.x - text.getLocalBounds().width / 2.0f, item.pos.y - text.getLocalBounds().height / 2.0f });
		}
		else
			text.setPosition(item.pos);

		text.setFillColor(item.color);

		window.draw(text);

	}
	textObjects.clear();
}

