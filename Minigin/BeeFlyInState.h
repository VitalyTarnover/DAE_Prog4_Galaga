#pragma once
#include "BaseEnemyState.h"

class BeeFlyInState final : public BaseEnemyState
{
public:
	BeeFlyInState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

