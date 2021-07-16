#pragma once
#include "BaseComponent.h"

//states: fly in, in formation, dive down, die
class BaseEnemyState;

class BaseEnemyMovementComponent : public BaseComponent
{
public:
	BaseEnemyMovementComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });//TODO: get rid of these nasty numbers after all the tests
	virtual ~BaseEnemyMovementComponent();
	virtual glm::vec2 GetPosInFormation();
	virtual float GetSpeed();
	virtual void Update();
	virtual void Switch();
protected:

	BaseEnemyState* m_CurrentState;
	glm::vec2 m_PosInFormation;
	float m_Speed;

	//TODO: Mby make getters and setters for these datamembers, this will be a good opportunity to make an adjustable difficulty
	//float m_StepTimer = 0;
	//float m_StepTime;
	//int m_StepsNumber;
	//int m_CurrentStepNumber = 0;
	//int m_StepSize;
	//bool m_MovingLeft = true;
};

