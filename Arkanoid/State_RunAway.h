#pragma once
#include "EnemyLevel1.h"

class State_RunAway :
	public State
{
public:
	void Execute(GameBaseEntity* entity, float dt)override
	{
		entity->State_RunAway(dt);

	}

};
