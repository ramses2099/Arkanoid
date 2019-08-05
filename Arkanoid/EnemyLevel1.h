#pragma once

#include "GameBaseEntity.h"
#include "State.h"
#include "Vec2D.h"
#include <iostream>

using namespace jp;
using namespace std;

class EnemyLevel1 :
	public GameBaseEntity
{
public:
	EnemyLevel1(int Id, Vec2D position, const std::string& path)
		:GameBaseEntity::GameBaseEntity(Id), _path(path)
	{
		this->_position = position;
		init();
	};
	~EnemyLevel1() {};

	void ChangeState(State* pNewStae)
	{
		delete this->m_CurrentState;
		this->m_CurrentState = pNewStae;
	}

	void init()override
	{
		if (_texture.loadFromFile(_path)) 
		{
			_sprite.setTexture(_texture);
			_sprite.setPosition(_position.sf());
		}
		//
		this->m_fMass = 10.f;
		this->m_force = 5.f;
		this->m_maxSpeed = 3.0f;
		//
		//_acceleration.set((this->m_force / this->m_fMass),0.f);

		_acceleration.set(0.5f, 0.f);
		
	}

	void draw(sf::RenderWindow& window)override 
	{
		window.draw(_sprite);	
	}

	//
	void update(float dt)override
	{
		this->m_CurrentState->Execute(this, dt);
	}
	//
	void State_RunAway(float dt)override
	{
		_acceleration.mult(dt);
		_velocity.add(_acceleration);
		_velocity.limit(m_maxSpeed);
		_position.add(_velocity);
		_sprite.setPosition(_position.sf());


		std::cout << " dt " << dt << std::endl;
		std::cout << " accel " << _acceleration.to_str() << std::endl;
		std::cout << " vel " << _velocity.to_str() << std::endl;
		std::cout << " pos " << _position.to_str() << std::endl;
		
	}

private:
	State* m_CurrentState = nullptr;

	const std::string& _path;
	sf::Sprite _sprite;
	sf::Texture _texture;

	Vec2D _position;
	Vec2D _acceleration;
	Vec2D _velocity;

	float  m_fMass;
	float m_force;
	float m_maxSpeed;

};

