#include "pch.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ballRadius{ 10.f }, ballVelocity{ 8.f };


//struct == class in c++
struct Ball
{
	CircleShape shape;
	Vector2f veloxity{ -ballVelocity, -ballVelocity };

	Ball(float x, float y)
	{
		shape.setPosition(x, y);
		shape.setRadius(ballRadius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(ballRadius, ballRadius);
	}

	void update() 
	{
		shape.move(veloxity); 

		if (left() < 0) 
		{
			veloxity.x = ballVelocity; 
		}
		else if (right() > windowWidth) 
		{
			veloxity.x = -ballVelocity; 
		}
		//
		if (top() < 0) 
		{
			veloxity.y = ballVelocity;
		}
		else if (bottom() > windowHeight)
		{
			veloxity.y = -ballVelocity;
		}


	}

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
	   	 
};

int main()
{

	Ball ball{ windowWidth / 2, windowHeight / 2 };


	RenderWindow window{ VideoMode(windowWidth, windowHeight),"Arkanoid" };
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		ball.update();


		window.clear();

		window.draw(ball.shape);
		window.display();
	}

	return 0;
}