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


using namespace std;
using namespace sf;
using namespace CompositionArkanoid;


constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ballRadius{ 10.f }, ballVelocity{ 8.f };

constexpr float paddleWidth{ 60.f }, paddleHeight{ 20.f }, paddleVelocity{ 6.f };
constexpr float blockWidth{ 60.f }, blockHeight{ 20.f };
constexpr int countBlocksX{ 11 }, countBlocksY{ 4 };

//is the same typedef
using FrameTime = float;

constexpr float ftStep{ 0.100f }, ftSlice{ 1.f };

//struct == class in c++
struct Game;

struct CPosition : Component
{
	Vector2f position;

	CPosition() = default;
	CPosition(const Vector2f& mPosition) :position(mPosition) {}

	float x() const noexcept { return position.x; }
	float y() const noexcept { return position.y; }
};

struct CPhysics :Component
{

	CPosition* cPosition{ nullptr };
	Vector2f velocity, halfSize;

	std::function<void(const Vector2f&)>onOutOfBounds;

	CPhysics(const Vector2f& mHalfSize) :halfSize(mHalfSize) {}

	void init()override
	{
		cPosition = &entity->getComponent<CPosition>();

	}
	//
	void update(float dt)override
	{
		cPosition->position += velocity * dt;

		if (onOutOfBounds == nullptr) return;

		if (left() < 0)
		{
			onOutOfBounds(Vector2f{ 1.f,0.f });
		}
		else if (right() > windowWidth)
		{
			onOutOfBounds(Vector2f{ -1.f,0.f });
		}
		//
		if (top() < 0)
		{
			onOutOfBounds(Vector2f{ 0.f,1.f });
		}
		else if (bottom() > windowHeight)
		{
			onOutOfBounds(Vector2f{ 0.f, -1.f });
		}


	}

	float x() const noexcept { return cPosition->x(); }
	float y() const noexcept { return cPosition->y(); }
	float left()const noexcept { return x() - halfSize.x; }
	float right()const noexcept { return x() + halfSize.x; }
	float top() const noexcept { return y() - halfSize.y; }
	float bottom() const noexcept { return y() + halfSize.y; }

};

struct CCircle : Component
{
	Game* game{ nullptr };
	CPosition* cPosition{ nullptr };
	CircleShape shape;
	float radius;

	CCircle(Game* mGame, float mRadius) :game(mGame), radius(mRadius) {}

	void init()override
	{
		cPosition = &entity->getComponent<CPosition>();
		//
		shape.setRadius(radius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(radius, radius);
	}
	//
	void update(float dt)override
	{
		shape.setPosition(cPosition->position);
	}
	//
	void draw(RenderWindow& window)override 
	{
		window.draw(shape);
	}

};

struct CRectangle : Component
{
	Game* game{ nullptr };
	CPosition* cPosition{ nullptr };
	RectangleShape shape;
	Vector2f size;

	CRectangle(Game* mGame, const Vector2f& mHalfSize) :game{ mGame }, size{ mHalfSize * 2.f }{};

	void init()override
	{
		cPosition = &entity->getComponent<CPosition>();
		//
		shape.setSize(size);
		shape.setFillColor(Color::Red);
		shape.setOrigin(size.x / 2.f, size.y / 2.f);
	}

	void update(float dt)override
	{
		shape.setPosition(cPosition->position);
	}
	//
	void draw(RenderWindow& window)override 
	{
		window.draw(shape);
	}

};

struct CPaddleControl :Component
{
	CPhysics* cPhysics{ nullptr };

	void init()override
	{
		cPhysics = &entity->getComponent<CPhysics>();

	}

	void update(float dt)override
	{
		if (Keyboard::isKeyPressed(Keyboard::Key::Left) && cPhysics->left() > 0)
		{
			cPhysics->velocity.x = -paddleVelocity;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && cPhysics->right() < windowWidth)
		{
			cPhysics->velocity.x = paddleVelocity;
		}
		else {
			cPhysics->velocity.x = 0;
		}

	}

};

template<class T1, class T2>bool isIntersecting(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right()
		&& mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

//test collition
void testCollisionPB(Entity& mPaddle, Entity& mBall)noexcept
{
	auto& cpPaddle(mPaddle.getComponent<CPhysics>());
	auto& cpBall(mBall.getComponent<CPhysics>());

	if (!isIntersecting(cpPaddle, cpBall))return;

	cpBall.velocity.y = -ballVelocity;

	if (cpBall.x() < cpPaddle.x())
		cpBall.velocity.x = -ballVelocity;
	else
		cpBall.velocity.x = ballVelocity;

}

//test collition
void testCollisionBB(Entity& mBrick, Entity& mBall)
{

	auto& cpBrick(mBrick.getComponent<CPhysics>());
	auto& cpBall(mBall.getComponent<CPhysics>());

	if (!isIntersecting(mBrick, mBall))return;
	mBrick.destroy();

	float overlapLeft{ cpBall.right() - cpBrick.left() };
	float overlapRight{ cpBrick.right() - cpBall.left() };
	float overlapTop{ cpBall.bottom() - cpBrick.top() };
	float overlapBottom{ cpBrick.bottom() - cpBall.top() };

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		cpBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
	else
		cpBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;

}

struct Game
{
	enum ArkanoidGroup : std::size_t
	{
		GPaddle,
		GBrick,
		GBall
	};


	RenderWindow window{ VideoMode(windowWidth, windowHeight),"Arkanoid" };
	FrameTime lastFt{ 0.f }, currentSlice{ 0.f };
	bool running{ false };
	Manager manager;

	Entity& createBall()
	{
		auto& entity(manager.addEntity());

		entity.addComponent<CPosition>(Vector2f{ windowWidth / 2.f,windowHeight / 2.f });
		entity.addComponent<CPhysics>(Vector2f{ ballRadius,ballRadius });
		entity.addComponent<CCircle>(this, ballRadius);

		auto& cPhysics(entity.getComponent<CPhysics>());
		cPhysics.velocity = Vector2f{ -ballVelocity,-ballVelocity };
		cPhysics.onOutOfBounds = [&cPhysics](const Vector2f& mSide)
		{
			if (mSide.x != 0.f)
				cPhysics.velocity.x = std::abs(cPhysics.velocity.x) * mSide.x;
			if (mSide.y != 0.f)
				cPhysics.velocity.y = std::abs(cPhysics.velocity.y) * mSide.y;

		};

		entity.addGroup(ArkanoidGroup::GBall);

		return entity;

	}

	Entity& createBrick(const Vector2f& mPosition)
	{
		Vector2f halfSize{ blockWidth / 2.f, blockHeight / 2.f };
		auto& entity(manager.addEntity());

		entity.addComponent<CPosition>(mPosition);
		entity.addComponent<CPhysics>(halfSize);
		entity.addComponent<CRectangle>(this, halfSize);

		entity.addGroup(ArkanoidGroup::GBrick);

		return entity;
	}
	
	Entity& createPaddle()
	{
		Vector2f halfSize{ blockWidth / 2.f, blockHeight / 2.f };
		auto& entity(manager.addEntity());

		entity.addComponent<CPosition>(Vector2f{ windowWidth / 2.f,windowHeight / 2.f });
		entity.addComponent<CPhysics>(halfSize);
		entity.addComponent<CRectangle>(this, halfSize);		
		entity.addComponent<CPaddleControl>();

		entity.addGroup(ArkanoidGroup::GPaddle);

		return entity;
	}
	

	Game()
	{
		createPaddle();
		createBall();

		for (int iX{ 0 }; iX < countBlocksX; ++iX)
		{
			for (int iY{ 0 }; iY < countBlocksY; ++iY)
			{
				createBrick(Vector2f{ (iX + 1)* (blockWidth + 3) + 22,
					(iY + 2) * (blockHeight + 3) });
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

			manager.refresh();
			manager.update(ftStep);

			auto& paddles(manager.getEntitiesByGroup(GPaddle));
			auto& briks(manager.getEntitiesByGroup(GBrick));
			auto& balls(manager.getEntitiesByGroup(GBall));

			//collition
			for (auto& b : balls) 
			{
				for (auto& p : paddles) 
				{
					testCollisionPB(*p, *b);
				}

				for (auto& br : briks) 
				{
					testCollisionBB(*br, *b);
				}
			
			}



		}

	}
	//
	void drawPhase()
	{
		window.clear();

		manager.draw(window);

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


//ECS example

struct CounterComponent :Component
{
	float counter;
	void update(float dt)override
	{
		counter += dt;
		std::cout << counter << std::endl;
	}

};


//ECS example
int main()
{
	//run gane
	Game{}.run();


	return 0;
}