#pragma once

class GameBaseEntity;
class State
{
public:
	virtual void Execute(GameBaseEntity* entity, float dt) = 0;
	
};
