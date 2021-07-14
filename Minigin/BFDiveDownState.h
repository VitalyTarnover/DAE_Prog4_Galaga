#pragma once
#include "BaseEnemyState.h"

class BFDiveDownState : public BaseEnemyState
{
public:
	BFDiveDownState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

