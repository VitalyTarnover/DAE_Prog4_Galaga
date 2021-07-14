#pragma once
#include "BaseEnemyState.h"

class InFormationState : public BaseEnemyState  //same for all enemies
{
public:
	InFormationState();
	virtual BaseEnemyState* Update(BaseEnemyMovementComponent& enemyMovement);
};

