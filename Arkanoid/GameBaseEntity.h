#pragma once
#include <SFML/Graphics.hpp>



class GameBaseEntity
{
public:
	GameBaseEntity(int id)
	{
		SetID(id);
	};
	~GameBaseEntity() {};

	int GetId() const noexcept { return this->m_ID; }
	
	virtual void init() {};
	virtual void update(float dt){};
	virtual void input(sf::Event event) {};
	virtual void draw(sf::RenderWindow& window) {};

	virtual void State_RunAway(float dt) {};

private:
	int m_ID;
		
	static int m_iNextValidID;

	inline void SetID(int val) { this->m_ID = val; }


};