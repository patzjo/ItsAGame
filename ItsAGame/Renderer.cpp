#include <iostream>

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Renderer.h"
#include "Level.h"
#include "World.h"

#include "Assets.h"
#include "Utils.h"



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

void Renderer::renderGameObjects()
{
	for (auto component : renderComponents)
	{
		for (auto object : component->graphics)
		{
			sf::Vector2f position = component->parent->position + object->offset;
			switch (object->type)
			{
			case RENDER_SPRITE:

				if (object->center)
					object->sprite.setOrigin(object->sprite.getLocalBounds().width*0.5f, object->sprite.getLocalBounds().height*0.5f);
				else
					object->sprite.setOrigin(object->origin);

				object->sprite.setRotation(object->angle);
				object->sprite.setPosition((int)position.x, (int)position.y);
				window.draw(object->sprite);
				break;

			case RENDER_SHAPE:
				switch (object->shape)
				{
				case CIRCLE:
				{
					sf::CircleShape shape;
					shape.setFillColor(object->fillColor);
					shape.setOutlineColor(object->outlineColor);
					shape.setRadius(object->radius);
					if (object->center)
						shape.setOrigin(object->radius, object->radius);
					else
						shape.setOrigin(object->origin);
					shape.setPosition((int)position.x, (int)position.y);
					window.draw(shape);

				} break;

				case RECTANGLE:
				{
					sf::RectangleShape shape;
					shape.setFillColor(object->fillColor);
					shape.setOutlineColor(object->outlineColor);
					shape.setOutlineThickness(object->outlineThickness);
					shape.setSize(object->rect.halfSize*2.0f);
					shape.setSize(object->rect.halfSize);
					if (object->center)
						shape.setOrigin(object->rect.halfSize);
					else
						shape.setOrigin(object->origin);
					shape.setRotation(object->angle);
					shape.setPosition((int)position.x, (int)position.y);

					window.draw(shape);

				} break;
				} break;
			}
		}
	}
}

void Renderer::pushRenderable(struct RenderComponent * renderComponent)
{
	if (renderComponent)
	{
		for (auto piece : renderComponent->graphics)
		{
			if ( piece->type == RENDER_SPRITE )
				piece->sprite.setTexture(*getTexture(piece->textureID));
		}
		
		renderComponents.push_back(renderComponent);
	}
}

void Renderer::removeRenderable(struct RenderComponent *renderComponent)
{
	for (auto itr = renderComponents.begin(); itr != renderComponents.end(); itr++)
	{
		if (*itr == renderComponent)
		{
			renderComponents.erase(itr);
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

	// For qTree debug drawing
	world->collisionTree.draw(this, sf::Color::Green, 1.0f);
	//drawLevelCollisionBoxes();
	
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

