#include <iostream>

#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Renderer.h"
#include "Level.h"
#include "World.h"

#include "Assets.h"
#include "Utils.h"

#include "Game.h"
#include "State.h"


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

	createBackground(2,2);

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



void Renderer::createBackground( int startTexture, int endTexture )
{
//	backgroundSprite.setTextureRect({ 0, 0, 1280, 1024 });
	/*
	for (unsigned int c = startTexture; c < endTexture; c++)
	{
	}
*/
	backgroundSprite.setTexture(*getTexture(startTexture));
	backgroundSprite.setColor({ 100, 100, 100 });
}

void Renderer::renderBackground()
{
	window.draw(backgroundSprite);
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

			case RENDER_ANIM:
			{
				sf::RenderStates state;
				state.texture = getTexture(object->textureID);
				window.draw(object->vertices, state); 
			}
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
			if (piece->type == RENDER_SPRITE)
				piece->sprite.setTexture(*getTexture(piece->textureID));

			else if (piece->type == RENDER_ANIM)
			{
				float tu = renderComponent->currentFrame % (int)(getTexture(piece->textureID)->getSize().x / renderComponent->animWidth);
				float tv = renderComponent->currentFrame / (int)(getTexture(piece->textureID)->getSize().y / renderComponent->animHeight);

				float halfW = renderComponent->animWidth / 2.0f;
				float halfH = renderComponent->animHeight / 2.0f;


				piece->vertices[0].position = { renderComponent->parent->position.x - halfW, renderComponent->parent->position.y - halfH };
				piece->vertices[1].position = { renderComponent->parent->position.x + halfW, renderComponent->parent->position.y - halfH };
				piece->vertices[2].position = { renderComponent->parent->position.x + halfW, renderComponent->parent->position.y + halfH };
				piece->vertices[3].position = { renderComponent->parent->position.x - halfW, renderComponent->parent->position.y + halfH };
				
				piece->vertices[0].texCoords = sf::Vector2f{ tu *     (float)renderComponent->animWidth, tv *		(float)renderComponent->animHeight };
				piece->vertices[1].texCoords = sf::Vector2f{ (tu+1) * (float)renderComponent->animWidth, tv *		(float)renderComponent->animHeight };
				piece->vertices[2].texCoords = sf::Vector2f{ (tu+1) * (float)renderComponent->animWidth, (tv+1) *	(float)renderComponent->animHeight };
				piece->vertices[3].texCoords = sf::Vector2f{ tu *     (float)renderComponent->animWidth, (tv+1) *	(float)renderComponent->animHeight };
			}
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

void Renderer::drawCollisionVolumes()
{
	sf::RectangleShape rect;
	sf::CircleShape circle;

	rect.setFillColor(sf::Color::Red);
	circle.setFillColor(sf::Color::Yellow);

	for (auto i :  renderComponents)
	{
		if (!i->parent->getCollisionComponent()) continue;

		if (i->parent->getCollisionComponent()->collisionMode == CIRCLE_COLLISION)
		{
			circle.setRadius(i->parent->getCollisionComponent()->circleCollisionRadius);
			circle.setOrigin(i->parent->getCollisionComponent()->circleCollisionRadius, i->parent->getCollisionComponent()->circleCollisionRadius);

			circle.setPosition(i->parent->position);

			window.draw(circle);
		}
		if (i->parent->getCollisionComponent()->collisionMode == BOX_COLLISION)
		{
			
			rect.setPosition(i->parent->position.x - i->parent->getCollisionComponent()->getCollisionBox().halfSize.x, 
				i->parent->position.y - i->parent->getCollisionComponent()->getCollisionBox().halfSize.y);
			rect.setSize(i->parent->getCollisionComponent()->getCollisionBox().halfSize * 2.0f);

			window.draw(rect);
		}
	}
}

void Renderer::render()
{
	window.clear();
	
	
	renderBackground();

	if ( world->getGame()->getState()->getState() == StateEnum::PLAYING )
	{
		renderLevel();
		renderGameObjects();
	}

	// For qTree debug drawing
	world->collisionTree.draw(this, sf::Color::Green, 1.0f);

	//drawLevelCollisionBoxes();
	//drawCollisionVolumes();

	renderTexts();
	window.display();
}

void Renderer::updateAnimations(float dT)
{
	for (auto component : renderComponents)
	{
		for (auto object : component->graphics)
		{
			if (object->type == RENDER_ANIM)
			{
				component->time++;
				if (component->time > component->timePerFrame)
				{
					component->time = 0.0f;
					component->currentFrame++;

					if (component->currentFrame >= component->numAnimFrames)
					{
						if (component->loop)
							component->currentFrame = 0;
						else
							world->notifySubject(E_REMOVE_GAMEOBJECT, (void *)component->parent);
					}
					int tu = component->currentFrame % (int)(getTexture(object->textureID)->getSize().x / component->animWidth);
					int tv = component->currentFrame / (int)(getTexture(object->textureID)->getSize().y / component->animHeight);


					object->vertices[0].texCoords = { tu * component->animWidth, tv * component->animHeight };
					object->vertices[1].texCoords = { (tu + 1) * component->animWidth, tv * component->animHeight };
					object->vertices[2].texCoords = { (tu + 1) * component->animWidth, (tv + 1) * component->animHeight };
					object->vertices[3].texCoords = { tu * component->animWidth, (tv + 1) * component->animHeight }; 
				}
				
			}
		}
	}
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
	std::string fontPath;
	std::string texturePath;

	fontPath = texturePath = assetsDirectory;
	fontPath += fontsDirectory;
	texturePath += texturesDirectory;
	
	for (unsigned int c = 0; c < sizeof(assets) / sizeof(assets[0]); c++)
	{
		switch (assets[c].type)
		{
		case FONT:
			addFont(fontPath + assets[c].filename);
			break;
		case TEXTURE:
			addTexture(texturePath + assets[c].filename);
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

