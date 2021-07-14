#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

//states: fly in, in formation, dive down, die

class BaseEnemyMovementComponent : public BaseComponent
{
public:
	BaseEnemyMovementComponent();
	virtual void Update();
protected:
	BaseEnemyState* m_CurrentState;


	//TODO: Mby make getters and setters for these datamembers, this will be a good opportunity to make an adjustable difficulty
	//float m_StepTimer = 0;
	//float m_StepTime;
	//int m_StepsNumber;
	//int m_CurrentStepNumber = 0;
	//int m_StepSize;
	//bool m_MovingLeft = true;
};

