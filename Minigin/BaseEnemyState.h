#pragma once
#include "BaseEnemyMovementComponent.h"

class BaseEnemyState
{
public:
	BaseEnemyState(GameObject* enemy);
	virtual ~BaseEnemyState() {};
	virtual BaseEnemyState* Update() {}
protected:
	GameObject* m_Enemy;
};

