#pragma once
#include "BaseEnemyState.h"

class BirdDiveDownState : public BaseEnemyState
{
public:
	BirdDiveDownState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

