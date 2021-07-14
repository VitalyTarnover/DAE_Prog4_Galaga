#pragma once
#include "BaseEnemyMovementComponent.h"

class BaseEnemyState
{
public:
	virtual ~BaseEnemyState() {}
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement) {}
};

