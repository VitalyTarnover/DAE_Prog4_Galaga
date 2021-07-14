#pragma once
#include "BaseEnemyState.h"

class BeeDiveDownState : public BaseEnemyState
{
public:
	BeeDiveDownState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

