#include "GameOptions.h"
#include "World.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Sound.h"
#include "Obspat.h"
#include "Particles.h"
#include "Level.h"
#include "Compat.h"
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
	while (status != GameStatus::QUIT)
	{
		switch (status)
		{
		case MENU:
			menuLoop();
			break;

		case PLAYING:
		case START_NEW_GAME:
			playLoop();
			break;

		default:break;
		}
	}

	renderer.getWindow().close();
}

void Game::processEvents(sf::RenderWindow & window)
{
	input.updateMousePosition(sf::Mouse::getPosition(renderer.getWindow()));
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			status = QUIT;
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				status = QUIT;
				break;

			case sf::Keyboard::Space:
				world.level.doCircleHole((sf::Vector2f)input.getMousePosition(), 100, sf::Color::Black);
				break;
			
			case sf::Keyboard::I:
				world.createObject((sf::Vector2f)input.getMousePosition(), new TestObject(&renderer));
				break;

			case sf::Keyboard::Q:
				world.collisionTree.toggleDebug();
				break;

			default: break;
			}
		}

	}

}

void Game::playLoop()
{
	if (status == START_NEW_GAME)
	{
		world.level.generateRectangleLevel(options.screenWidth, options.screenHeight, 200, 600, 100, 200, sf::Color::Blue);
		status = PLAYING;
	}
	int frames = 0;
	int framesPerSec = 0;
	float timeElapsed = 0.0f;

	sf::Clock clock;
	while (status == PLAYING)
	{
		float deltaTime = clock.restart().asSeconds();
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
	}

}

void Game::menuLoop()
{
	while (status == MENU)
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
	renderer.pushText(text, { 0.0f, 0.0f }, 0, 32, sf::Color::White);
}

