#pragma once
#include "ECS.h"
#include "Vec2D.h"

using namespace jp;

namespace ecs
{

	class PositionComponent:public Component
	{
	public:
		PositionComponent() {};
		PositionComponent(float x, float y) 
		{
			_position = Vec2D(x, y);
		};
		~PositionComponent() {};

		void init()override 
		{
			_velocity = Vec2D(2.f, 1.f);
			_acceleration = Vec2D(2.f, 1.f);

		}
		//
		void update(float dt)override 
		{
			_acceleration.mult(dt);
			_velocity.add(_acceleration);
			_position.add(_velocity);
		
		}
		//	
		void setPosition(Vec2D v) 
		{
			_position = v;
		}

	private:
		Vec2D _position;
		Vec2D _velocity;
		Vec2D _acceleration;
	};


};