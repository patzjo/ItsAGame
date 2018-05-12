#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include <map>

enum RendableType { RENDER_SPRITE, RENDER_ANIM, RENDER_SHAPE };

struct RendererTextObject
{
	sf::Vector2f pos;
	int fontID;
	int characterSize;
	sf::Color color;
	
	std::string text;
	bool centered;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Initialize renderer
	bool initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vsync = true, bool fullScreen = false);

	// Adds new texture to renderer
	void addTexture(std::string filename);
	// Adds new font to renderer
	void addFont(std::string filename);
	
	// Push text to renderer
	void pushText(std::string text, sf::Vector2f pos, int fontID, int characterSize, sf::Color color, bool centered = false);
	
	// Load Assets.h assets
	void loadGraphicAssets();

	// Return font from fonts
	sf::Font *getFont(int id)		{ return &fonts[id]; }
	// Return texture from textures
	sf::Texture *getTexture(int id)  { return &textures[id]; }
	// Return window
	sf::RenderWindow& getWindow()	{ return window; }
	
	void createBackground(int startTexture, int endTexture);

	// Render background
	void renderBackground();
	// Renders level
	void renderLevel();
	// Renders all gameobjects
	void renderGameObjects();
	// Renders all texts
	void renderTexts();

	// Adds pointers to objects
	void setLevelPointer(class Level *lev)  { level = lev; }
	void setWorldPointer(class World *wrld) { world = wrld; }

	// Register RenderComponent to renderer
	void pushRenderable(struct RenderComponent *renderComponent);

	// Remove RenderComponent from renderer
	void removeRenderable(struct RenderComponent * renderComponent);

	// Debug draw level collisionBoxes.
	void drawLevelCollisionBoxes();
	void drawCollisionVolumes();

	// Render
	void render();

	// Close renderer
	void shutDown() { window.close(); }

	void updateAnimations(float dT);



private:
	std::vector <sf::Texture>		       textures;
	std::vector <sf::Font>			       fonts;
	std::vector <RendererTextObject>	   textObjects;
	std::vector <class GameObject *>	   renderObjects;
	std::vector <struct RenderComponent *> renderComponents;

	sf::Sprite levelSprite;
	sf::Sprite backgroundSprite;
	sf::RenderWindow window;

	class Level *level = nullptr;
	class World *world = nullptr;
};

