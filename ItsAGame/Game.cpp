#include "GameOptions.h"
#include "World.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Sound.h"
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
}

void Game::run()
{
	sf::Text text;
	
	sf::Clock clock;
	while (state.getState() != StateEnum::QUIT)
	{
		switch (state.getState())
		{
		case StateEnum::MENU:
			menuLoop();
			break;

		case StateEnum::PLAYING:
		case StateEnum::START_NEW_GAME:
			playLoop();
			break;

		default:break;
		}
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


			default: break;
			}
		}
	}

	input.processInput(this, players[0]);

}

void Game::playLoop()
{
	if (state.getState() == StateEnum::START_NEW_GAME)
	{
		world.level.generateRectangleLevel(options.screenWidth, options.screenHeight, 200, 600, 100, 200, sf::Color::Blue);
		world.collisionTree.setLevelCollisionBoxes(world.level.getLevelCollisionBoxes());

		state.setState(StateEnum::PLAYING);
	}

	int frames = 0;
	int framesPerSec = 0;
	float timeElapsed = 0.0f;

	sf::Clock clock;
	bool firstFrame = true;
	while (state.getState() == StateEnum::PLAYING)
	{
		deltaTime = clock.restart().asSeconds();
		processEvents(renderer.getWindow());
		
		world.update(deltaTime);

		showFPS(framesPerSec);

		renderer.render();
		
		timeElapsed += deltaTime;
		frames++;
		if (timeElapsed >= 1.0)
		{
			timeElapsed = 0.0f;
			framesPerSec = frames;
			frames = 0;
		}

		if (!firstFrame)
		{
			if (deltaTime > deltaTimeMax)
				deltaTimeMax = deltaTime;

			if (deltaTime < deltaTimeMin)
				deltaTimeMin = deltaTime;
		}

		firstFrame = false;
	}

}

void Game::menuLoop()
{
	while (state.getState() == StateEnum::MENU)
	{
		processEvents(renderer.getWindow());
		renderer.render();
	}
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

	renderer.pushText(text, { 0.0f, 0.0f }, 0, 32, sf::Color::White);
}

