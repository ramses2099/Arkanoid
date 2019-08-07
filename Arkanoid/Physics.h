#pragma once

#include "Vec2D.h"

using namespace jp;

struct Physics
{

	Physics():_position(0.f,0.f),_velocity(0.f, 0.f),
	_acceleration(0.f, 0.f),_steer(0.f, 0.f),_maxSpeed(0.f),_maxForce(0.f) {}

	Vec2D _position;
	Vec2D _velocity;
	Vec2D _acceleration;
	Vec2D _steer;

	float _maxSpeed;
	float _maxForce;
	

};