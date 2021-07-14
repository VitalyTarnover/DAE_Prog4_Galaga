#pragma once
#include "BaseEnemyState.h"


class DeadState	: public BaseEnemyState  //same for all enemies
{
public:
	DeadState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

