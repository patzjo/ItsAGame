#pragma once

#include "World.h"
#include "GameOptions.h"
#include "Input.h"
#include "Renderer.h"
#include "State.h"
#include "Noise.h"

class Game
{
	friend class World;
public:
	Game();
	~Game();

	void initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vSync = true, bool fullScreen = false);
	void run();

	void exit() { state.setState(StateEnum::QUIT); }
	void processEvents(sf::RenderWindow& window);

	void playLoop( float deltaTime );
	void menuLoop();

	void showFPS(int frameCount);
	void toggleFPS() { FPSCounter = !FPSCounter; }

	class World *getWorld() { return &world; }
	float getDeltaTime() { return deltaTime; }
	int getPlayerCount() { return playerCount;  }

	class Player *getPlayer(int index) { return players[index]; }
	struct GameOptions *getOptions() { return &options; }
	void updateGameSituation(PlayerKilledData *data);
private:
	struct GameOptions options;
	class Input input;
	class World world;
	class Renderer renderer;
	class State state;
	class Noise sound;

	class Player *players[4]{ nullptr };
	int   playerCount = 0;

	float deltaTime = 0.0f;
	float deltaTimeMin = 1.0f;
	float deltaTimeMax = 0.0f;

	bool FPSCounter = true;
};

