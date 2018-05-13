#pragma once

#include "Menu.h"
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

	void initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vSync = true, bool fullScreen = false);	// Initialize game
	void run();	// Start game

	void exit() { state.setState(StateEnum::QUIT); }	// Exit game
	void processEvents(sf::RenderWindow& window);		// Process all window events.

	void playLoop( float deltaTime );					// Actual game loop
	void menuLoop( float deltaTime );					// Actual menu loop

	void showFPS(int frameCount);						// Show fps (And whew other informations)
	void toggleFPS() { FPSCounter = !FPSCounter; }		// Toggle showing fps

	class World *getWorld() { return &world; }			// Return world
	float getDeltaTime() { return deltaTime; }			// Return deltaTime
	int getPlayerCount() { return playerCount;  }		// Return player count

	class Player *getPlayer(int index) { return players[index]; }	// Return player from index
	struct GameOptions *getOptions() { return &options; }			// Get options
	void updateGameSituation(PlayerKilledData *data);				// Decide if game is over

	State *getState() { return &state; }

private:
	struct GameOptions options;		// Options
	class Input input;				// Inputs
	class World world;				// Actual world
	class Renderer renderer;		// Renderer
	class State state;				// State machine
	class Noise sound;				// Sounds
	class Menu menu;				// Menu

	class Player *players[4]{ nullptr };	// Place for four players
	int scores[4]{ 0 };
	int   playerCount = 0;		// How many players have been created

	float deltaTime = 0.0f;
	float deltaTimeMin = 1.0f;	// Keeps minimum loop time
	float deltaTimeMax = 0.0f;  // Keeps maximum loop time

	bool FPSCounter = false;		// Show fps counter ?
	std::string roundWinner = "";

};

