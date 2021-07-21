#pragma once
#include "BaseEnemyState.h"

class InFormationState : public BaseEnemyState  //same for all enemies
{
public:
	InFormationState();
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void Patrol(GameObject* enemy);

	bool m_StepSizeInitialized = false;
	int m_StepSize = 0;//not initialized value
};

