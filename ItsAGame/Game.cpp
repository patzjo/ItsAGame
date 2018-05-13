#include "GameOptions.h"
#include "World.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Noise.h"
#include "Obspat.h"
#include "Particles.h"
#include "Level.h"
#include "Command.h"
#include "Input.h"
#include "Game.h"
Game::Game()
{
}

Game::~Game()
{
}

void Game::initialize(int ScreenWidth, int ScreenHeight, std::string name, bool vSync, bool fullScreen)
{
	renderer.initialize(ScreenWidth, ScreenHeight, name, vSync, fullScreen);
	world.setGamePointer(this);
	
	renderer.setWorldPointer(&world);
	renderer.setLevelPointer(&world.level);

	world.initialize();
	sound.initialize();
}

void Game::run()
{
	sf::Text text;
	
	int frames = 0;
	int framesPerSec = 0;
	float timeElapsed = 0.0f;
	bool firstFrame = false;
	
	sf::Clock clock;
	while (state.getState() != StateEnum::QUIT)
	{
		deltaTime = clock.restart().asSeconds();
		state.changeRequestedState();

		switch (state.getState())
		{
		case StateEnum::MENU:
			menuLoop();
			break;

		case StateEnum::START_NEW_GAME:
			world.notifySubject(E_START_AGAIN, 0);
			world.level.generateRectangleLevel(options.screenWidth, options.screenHeight, 200, 600, 100, 200, sf::Color::Blue);
			world.collisionTree.setLevelCollisionBoxes(world.level.getLevelCollisionBoxes());
			world.startAgain();
			state.setState(StateEnum::PLAYING);
			firstFrame = true;
		break;


		case StateEnum::PLAYING:
			playLoop(deltaTime);
			break;

		default:break;
		}

		
		if (!firstFrame)
		{
			if (deltaTime > deltaTimeMax)
				deltaTimeMax = deltaTime;

			if (deltaTime < deltaTimeMin)
				deltaTimeMin = deltaTime;
		}

		// Update frames per seconds
		timeElapsed += deltaTime;
		frames++;
		if (timeElapsed >= 1.0)
		{
			timeElapsed = 0.0f;
			framesPerSec = frames;
			frames = 0;
		}


		showFPS(framesPerSec);
		renderer.render();
		world.processQueues();
		firstFrame = false;
	}

	renderer.shutDown();
}

void Game::processEvents(sf::RenderWindow & window)
{
	input.updateMousePosition(sf::Mouse::getPosition(renderer.getWindow()));
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			state.setState(StateEnum::QUIT);
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				state.setState(StateEnum::QUIT);
				break;

			case sf::Keyboard::I:
				world.createObject((sf::Vector2f)input.getMousePosition(), new CannonBall());
				break;

			case sf::Keyboard::Q:
				world.collisionTree.toggleDebug();
				break;

			case sf::Keyboard::F1:
				state.requestState(StateEnum::START_NEW_GAME);
				break;

			default: break;
			}
		}
	}

	for (unsigned int c = 0; c < playerCount; c++ )
		input.processInput(this, players[c]);

}

void Game::playLoop( float deltaTime )
{
		processEvents(renderer.getWindow());
		world.update(deltaTime);
		renderer.updateAnimations(deltaTime);
}

void Game::menuLoop()
{
	processEvents(renderer.getWindow());
	renderer.render();
}

void Game::showFPS(int frameCount)
{
	if (!FPSCounter)
		return;
	
	std::string text;
	text = "FPS: ";
	text += std::to_string(frameCount);

	text += " Objects: ";
	text += std::to_string(world.gameObjects.size());

	text += " DeltaTimeMin: ";
	text += std::to_string(deltaTimeMin);

	text += " DeltaTimeMax: ";
	text += std::to_string(deltaTimeMax);
/*
	text += " Falling: ";
	text += players[0]->falling ? "Yes" : "No";
*/
	renderer.pushText(text, { 0.0f, 0.0f }, 0, 32, sf::Color::White);
}

void Game::updateGameSituation(PlayerKilledData * data)
{
	int alivePlayers = 0;
	for (unsigned int c = 0; c < playerCount; c++)
		if (players[c])
			alivePlayers++;

	if (alivePlayers <= 1 && state.getState() == StateEnum::PLAYING)
	{
		state.requestState(START_NEW_GAME);
		//		state.setState(START_NEW_GAME);
	}
}

