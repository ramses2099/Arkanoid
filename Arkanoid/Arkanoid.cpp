#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include<chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "ECS.h"
#include "Components.h"
#include "EnemyLevel1.h"
#include "State_RunAway.h"
#include "NPC.h"


using namespace std;
using namespace sf;
using namespace ecs;
using namespace npc;

constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ftStep{ 0.16f }, ftSlice{ 1.f };

Manager manager;
auto& player(manager.addEntity());


struct Game
{

	RenderWindow window{ VideoMode(windowWidth, windowHeight),"Arkanoid" };
	bool running{ false };

	Clock frameClock;
	const Time timerPerFrame = seconds(1.0f / 60.0f);
	Time timeSinceLastUpdate = Time::Zero;
	   
	EnemyLevel1* enemy1;
	//State_RunAway* stateRunAway;
	 
	Vehicle* _vehicle;

	Game()
	{

		window.setFramerateLimit(60);

		//stateRunAway = new State_RunAway();
		enemy1 = new EnemyLevel1(1, Vec2D(550.f, 500.f), "assets/graphics/enemy001.png");

		enemy1->ChangeState(new State_RunAway());

		player.addComponent<PositionComponent>();

		_vehicle = new Vehicle(25.f, 25.f);

	}
	//
	void inputPhase()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			_vehicle->input(event);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))running = false;


		manager.input(event);

	}
	//
	void updatePhase(float dt)
	{
		manager.update(dt);

		enemy1->update(dt);

		//_vehicle->seek(enemy1->getPosition());

		_vehicle->arrive(enemy1->getPosition());

		_vehicle->update(dt);

	}
	//
	void drawPhase()
	{
		window.clear();

		enemy1->draw(window);

		manager.draw(window);

		_vehicle->draw(window);

		window.display();


	}
	//
	void run()
	{
		running = true;

		while (running)
		{
			timeSinceLastUpdate += frameClock.restart();

			window.clear();

			inputPhase();

			//update
			while (timeSinceLastUpdate > timerPerFrame) 
			{
				timeSinceLastUpdate -= timerPerFrame;

				updatePhase(timerPerFrame.asSeconds());
			}
			drawPhase();
					   
		}

	}

};


//ECS example
int main()
{
	//run gane
	Game{}.run();


	return 0;
}