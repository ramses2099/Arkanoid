#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include<chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "CES.h"
#include "EnemyLevel1.h"
#include "State_RunAway.h"


using namespace std;
using namespace sf;
using namespace CompositionArkanoid;


constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ftStep{ 0.16f }, ftSlice{ 1.f };

struct Game
{

	RenderWindow window{ VideoMode(windowWidth, windowHeight),"Arkanoid" };
	bool running{ false };

	Clock frameClock;
	const Time timerPerFrame = seconds(1.0f / 60.0f);
	Time timeSinceLastUpdate = Time::Zero;



	EnemyLevel1* enemy1;
	//State_RunAway* stateRunAway;



	Game()
	{

		window.setFramerateLimit(60);

		//stateRunAway = new State_RunAway();
		enemy1 = new EnemyLevel1(1, Vec2D(250.f, 300.f), "assets/graphics/enemy001.png");

		enemy1->ChangeState(new State_RunAway());

	}
	//
	void inputPhase()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))running = false;

	}
	//
	void updatePhase(float dt)
	{

		enemy1->update(dt);



	}
	//
	void drawPhase()
	{
		window.clear();

		enemy1->draw(window);

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