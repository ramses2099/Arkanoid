#pragma once


#include <SFML/Graphics.hpp>
#include "Vec2D.h"
#include "MathUtils.h"

using namespace jp;
using namespace sf;

namespace npc {


	class Vehicle
	{
	public:
		Vehicle() :_position(0.f, 0.f)
		{

		}
		//
		Vehicle(float x, float y)
		{
			_position = Vec2D(x, y);
			_velocity = Vec2D(0.f, 0.f);
			_acceleration = Vec2D(0.2f, 0.3f);

			_maxForce = 0.1f;
			_maxSpeed = 4;
			//
			loadSprite();

			_sprite.setPosition(_position.sf());
		}
		//
		~Vehicle() {};
		//
		void seek(Vec2D& target)
		{
			Vec2D desired = Vec2D{}.sub(target, _position);
			desired.normalize();
			desired.mult(_maxSpeed);
			Vec2D steer = Vec2D{}.sub(desired, _velocity);
			steer.limit(_maxForce);
			applayForce(steer);
		}
		//
		void arrive(Vec2D& target)
		{
			Vec2D desired = Vec2D{}.sub(target, _position);
			float d = desired.length();

			desired.normalize();

			if (d < 100)
			{
				float m = MathUtils{}.map(d, 0, 100, 0, _maxSpeed);
				desired.mult(m);
			}
			else {
				desired.mult(_maxSpeed);
			}

			Vec2D steer = Vec2D{}.sub(desired, _velocity);
			steer.limit(_maxForce);
			applayForce(steer);

		}
		//	   
		void input(Event event)
		{
			if (Keyboard::isKeyPressed(Keyboard::Key::Up))
			{
				applayForce(Vec2D(0.f, -0.5f));
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::Down))
			{
				applayForce(Vec2D(0.f, 0.5f));
			}

		}
		//
		void update(float dt)
		{
			_velocity.add(_acceleration);
			_velocity.limit(_maxSpeed);
			_position.add(_velocity);
			_sprite.setPosition(_position.sf());
			_acceleration.mult(0);

		}
		//
		void draw(RenderWindow& window)
		{
			window.draw(_sprite);
		}

	private:
		Vec2D _position;
		Vec2D _velocity;
		Vec2D _acceleration;
		Vec2D _steer;

		float _maxSpeed;
		float _maxForce;

		Sprite _sprite;
		Texture _texture;

		//
		void loadSprite()
		{
			if (_texture.loadFromFile("assets/graphics/enemy002.png"))
			{
				_sprite.setTexture(_texture);
			}
		}
		//
		void applayForce(Vec2D v)
		{
			_acceleration.add(v);
		}
		//

	};



};

