#pragma once
#include "BaseEnemyState.h"

class InFormationState : public BaseEnemyState  //same for all enemies but birds
{
public:
	InFormationState();
	virtual BaseEnemyState* Update(GameObject* enemy) override;
protected:
	virtual void SetSpriteState(GameObject* enemy) const;
	virtual void Patrol(GameObject* enemy) const;
	void InitializeStepSize(GameObject* enemy);

	bool m_StepSizeInitialized = false;
	int m_StepSize = 0;//not initialized value
};

