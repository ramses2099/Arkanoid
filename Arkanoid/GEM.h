#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <SFML/Graphics.hpp>

/*
* design ("inheritance") does not allow composition
*				[GameElement]
*	                   |
* [enemy]------------------------------[npc]
*/

using namespace std;
using namespace sf;

struct GameElement
{
	virtual void update(float dt) {};
	virtual void draw(RenderWindow& window) {};
	virtual ~GameElement() {};
};
//
struct Ball:GameElement
{
	void update(float dt)override {}
	void draw(RenderWindow& window)override {}
};
//
struct Brick :GameElement
{
	void update(float dt)override {}
	void draw(RenderWindow& window)override {}
};
//
struct Paddle :GameElement
{
	void update(float dt)override {}
	void draw(RenderWindow& window)override {}
};
//
struct Powerup :GameElement
{
	void update(float dt)override {}
	void draw(RenderWindow& window)override {}
};
//

struct Game
{
	std::vector<std::unique_ptr<GameElement>> elements;


	void update(float dt)
	{
		for (auto& e : elements)
		{
			e->update(dt);
		}

	}
	//
	void draw(RenderWindow& window)
	{
	
		for (auto& e : elements)
		{
			e->draw(window);
		}
	}

};