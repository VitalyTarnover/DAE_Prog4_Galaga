#pragma once
#include "BaseEnemyMovementComponent.h"

class BaseEnemyState
{
public:
	BaseEnemyState();
	virtual ~BaseEnemyState() {}
	virtual BaseEnemyState* Update(GameObject*) { return nullptr; }
	virtual void Switch();
protected:
	bool m_Switch;//switches to next state
};

