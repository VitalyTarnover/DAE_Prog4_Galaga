#pragma once
#include "BaseEnemyState.h"

class BirdFlyInState : public BaseEnemyState
{
public:
	BirdFlyInState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

