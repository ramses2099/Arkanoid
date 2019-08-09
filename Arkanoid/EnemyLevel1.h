#pragma once

#include "GameBaseEntity.h"
#include "State.h"
#include "Physics.h"
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
		this->_physics._position = position;
		init();
	};
	~EnemyLevel1() {};

	Vec2D& getPosition()noexcept 
	{
		return this->_physics._position;
	}


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
			_sprite.setPosition(this->_physics._position.sf());
		}
		//
		this->_physics._maxForce = 0.1f;
		this->_physics._maxSpeed = 4.0f;
		//
				
		applayForce(Vec2D(4.f, 0.f));

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
		this->_physics._acceleration.mult(dt);
		
		this->_physics._velocity.add(this->_physics._acceleration);
		this->_physics._velocity.limit(this->_physics._maxSpeed);
		this->_physics._position.add(this->_physics._velocity);
		_sprite.setPosition(this->_physics._position.sf());

		/*--
		std::cout << " dt " << dt << std::endl;
		std::cout << " accel " << this->_physics._acceleration.to_str() << std::endl;
		std::cout << " vel " << this->_physics._velocity.to_str() << std::endl;
		std::cout << " pos " << this->_physics._position.to_str() << std::endl;
		--*/
	}
	//
	void applayForce(Vec2D force)
	{
		this->_physics._acceleration.add(force);
	}


private:
	State* m_CurrentState = nullptr;

	const std::string& _path;
	sf::Sprite _sprite;
	sf::Texture _texture;

	//physics
	Physics _physics;
		
};

