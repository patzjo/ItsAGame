#pragma once

#include "World.h"
#include "GameOptions.h"
#include "Input.h"
#include "Renderer.h"

enum GameStatus {QUIT, MENU, PLAYING, START_NEW_GAME};

class Game
{
	friend class World;
public:
	Game();
	~Game();

	void initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vSync = false, bool fullScreen = false);
	void run();

	void exit() { status = QUIT; };
	void processEvents(sf::RenderWindow& window);

	void playLoop();
	void menuLoop();

private:
	struct GameOptions options;
	class Input input;
	class World world;
	class Renderer renderer;

	int status = START_NEW_GAME;
};

