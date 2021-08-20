#pragma once
#include "BaseEnemyBehaviorComponent.h"

class BaseEnemyState
{
public:
	BaseEnemyState();
	virtual ~BaseEnemyState() {}
	virtual BaseEnemyState* Update(GameObject*) { return nullptr; }
	virtual void Switch();
	virtual void Die(GameObject* enemy);
protected:
	bool m_Switch;//switches to next state
};

