#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include<chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ballRadius{ 10.f }, ballVelocity{ 8.f };

constexpr float paddleWidth{ 60.f }, paddleHeight{ 20.f }, paddleVelocity{ 6.f };
constexpr float blockWidth{ 60.f }, blockHeight{ 20.f };
constexpr int countBlocksX{ 11 }, countBlocksY{ 4 };

//is the same typedef
using FrameTime = float;

constexpr float ftStep{ 0.100f }, ftSlice{ 1.f };

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

	void update(FrameTime mFt)
	{
		shape.move(veloxity * mFt);

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

	float x() const noexcept { return shape.getPosition().x; }
	float y() const noexcept { return shape.getPosition().y; }
	float left()const noexcept { return x() - shape.getRadius(); }
	float right()const noexcept { return x() + shape.getRadius(); }
	float top() const noexcept { return y() - shape.getRadius(); }
	float bottom() const noexcept { return y() + shape.getRadius(); }

};

struct Rectangle
{
	RectangleShape shape;
	float x() const noexcept { return shape.getPosition().x; }
	float y() const noexcept { return shape.getPosition().y; }
	float left() const noexcept { return x() - shape.getSize().x / 2.f; }
	float right() const noexcept { return x() + shape.getSize().x / 2.f; }
	float top() const noexcept { return y() - shape.getSize().y / 2.f; }
	float bottom() const noexcept { return y() + shape.getSize().y / 2.f; }
};

struct Paddle :public Rectangle
{

	Vector2f velocity;

	Paddle(float x, float y)
	{

		shape.setPosition(x, y);
		shape.setSize({ paddleWidth,paddleHeight });
		shape.setFillColor(Color::Red);
		shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
	}

	void update(FrameTime mFt)
	{
		shape.move(velocity * mFt);

		if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0)
		{
			velocity.x = -paddleVelocity;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < windowWidth)
		{
			velocity.x = paddleVelocity;
		}
		else {
			velocity.x = 0;
		}

	}

};

struct Brick : public Rectangle
{

	bool destroyed{ false };

	Brick(float x, float y)
	{

		shape.setPosition(x, y);
		shape.setSize({ blockWidth,blockHeight });
		shape.setFillColor(Color::Yellow);
		shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
	}

};

template<class T1, class T2>bool isIntersecting(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right()
		&& mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

//test collition
void testCollision(Paddle& mPaddle, Ball& mBall)
{
	if (!isIntersecting(mPaddle, mBall))return;

	mBall.veloxity.y = -ballVelocity;

	if (mBall.x() < mPaddle.x())
		mBall.veloxity.x = -ballVelocity;
	else
		mBall.veloxity.x = ballVelocity;

}

//test collition
void testCollision(Brick& mBrick, Ball& mBall)
{
	if (!isIntersecting(mBrick, mBall))return;

	mBrick.destroyed = true;

	float overlapLeft{ mBall.right() - mBrick.left() };
	float overlapRight{ mBrick.right() - mBall.left() };
	float overlapTop{ mBall.bottom() - mBrick.top() };
	float overlapBottom{ mBrick.bottom() - mBall.top() };

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		mBall.veloxity.x = ballFromLeft ? -ballVelocity : ballVelocity;
	else
		mBall.veloxity.y = ballFromTop ? -ballVelocity : ballVelocity;

}

struct Game
{
	RenderWindow window{ VideoMode(windowWidth, windowHeight),"Arkanoid" };
	FrameTime lastFt{ 0.f }, currentSlice{ 0.f };
	bool running{ false };

	Ball ball{ windowWidth / 2, windowHeight / 2 };
	Paddle paddle{ windowWidth / 2, windowHeight - 50 };
	vector<Brick> bricks;



	Game()
	{
		for (int iX{ 0 }; iX < countBlocksX; ++iX)
		{
			for (int iY{ 0 }; iY < countBlocksY; ++iY)
			{
				bricks.emplace_back((iX + 1)* (blockWidth + 3) + 22,
					(iY + 2) * (blockHeight + 3));
			}
		}

		window.setFramerateLimit(60);

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
	void updatePhase()
	{
		currentSlice += lastFt;


		for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
		{

			ball.update(ftStep);
			paddle.update(ftStep);

			//collition
			testCollision(paddle, ball);

			for (auto& brick : bricks)
			{
				testCollision(brick, ball);
			}

			bricks.erase(remove_if(begin(bricks), end(bricks),
				[](const Brick& mbrick) {return mbrick.destroyed; }), end(bricks));


		}

	}
	//
	void drawPhase()
	{
		window.clear();

		window.draw(ball.shape);
		window.draw(paddle.shape);

		for (auto& brick : bricks)
		{
			window.draw(brick.shape);
		}

		window.display();


	}
	//
	void run()
	{
		running = true;

		while (running)
		{
			//use chrono
			auto timePoint1(chrono::high_resolution_clock::now());

			window.clear();

			inputPhase();
			updatePhase();
			drawPhase();


			auto timePoint2(chrono::high_resolution_clock::now());
			auto elapsedTime(timePoint2 - timePoint1);
			FrameTime ft{ chrono::duration_cast<
				chrono::duration<float, milli>>(elapsedTime).count() };

			lastFt = ft;

			auto ftSeconds(ft / 1000.f);
			auto fps(1.f / ftSeconds);


			window.setTitle("FT: " + to_string(ft) + "\tFPS: " + to_string(fps));
			
		}
		
	}
	   
};


int main()
{
	//run gane
	Game{}.run();

	return 0;
}