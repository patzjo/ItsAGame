#include <iostream>

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Renderer.h"
#include "Level.h"
#include "World.h"


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

	addFont("arial.ttf");
	return false;
}

void Renderer::addSprite(int id, sf::Sprite* sprite)
{
	sprites[id] = sprite;
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

