#pragma once
#include "BaseEnemyState.h"

class BFFlyInState : public BaseEnemyState
{
public:
	BFFlyInState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

