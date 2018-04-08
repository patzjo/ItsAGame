#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include <map>

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

	bool initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vsync = false, bool fullScreen = false);

	void addTexture(std::string filename);
	void addFont(std::string filename);
	void pushText(std::string text, sf::Vector2f pos, int fontID, int characterSize, sf::Color color, bool centered = false);
	
	void loadGraphicAssets();

	sf::Font *getFont(int id)		{ return &fonts[id]; }
	sf::Texture *getTexture(int id) { return &textures[id]; }
	sf::RenderWindow& getWindow()	{ return window; }
	
	void renderLevel();
	void renderGameObjects() {};
	void renderTexts();

	void setLevelPointer(class Level *lev) { level = lev; }
	void setWorldPointer(class World *wrld) { world = wrld; }

	void pushRenderable(struct RenderComponent *renderComponent);

	void removeRenderable(RenderComponent * renderComponent);

	void drawLevelCollisionBoxes();
	
	void render();

private:
	std::vector <sf::Texture>		       textures;
	std::vector <sf::Font>			       fonts;
	std::vector <RendererTextObject>	   textObjects;
	std::vector <class GameObject *>	   renderObjects;
	std::vector <struct RenderComponent *> renderComponents;

	sf::Sprite levelSprite;
	sf::RenderWindow window;

	class Level *level = nullptr;
	class World *world = nullptr;
};

