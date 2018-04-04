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

	void addSprite(int id, sf::Sprite *sprite);
	void addFont(std::string filename);
	void pushText(std::string text, sf::Vector2f pos, int fontID, int characterSize, sf::Color color, bool centered = false);
	
	sf::Font *getFont(int id) { return &fonts[id]; }
	sf::RenderWindow& getWindow() { return window; }
	
	
	void renderLevel();
	void renderGameObjects() {};
	void renderTexts();

	void setLevelPointer(class Level *lev) { level = lev; }
	void setWorldPointer(class World *wrld) { world = wrld; }

	void pushRenderable(struct RenderComponent *renderComponent);

	void removeRenderable(RenderComponent * renderComponent);
	
	void render();
private:

	std::map <int, sf::Sprite*>		       sprites;
	std::vector <sf::Font>			       fonts;
	std::vector <RendererTextObject>	   textObjects;
	std::vector <class GameObject *>	   renderObjects;
	std::vector <struct RenderComponent *> renderComponents;

	sf::Sprite levelSprite;
	sf::RenderWindow window;


	class Level *level = nullptr;
	class World *world = nullptr;

};

