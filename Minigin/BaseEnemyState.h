#pragma once
#include "BaseEnemyMovementComponent.h"

class BaseEnemyState
{
public:
	BaseEnemyState(GameObject* enemy);
	virtual ~BaseEnemyState() {}
	virtual BaseEnemyState* Update() {}
	virtual void Switch();
protected:
	GameObject* m_Enemy;
	bool m_Switch;//switches to next state
};

