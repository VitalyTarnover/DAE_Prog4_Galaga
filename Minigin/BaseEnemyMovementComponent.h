#pragma once
#include "BaseComponent.h"

//states: fly in, in formation, dive down, die
class BaseEnemyState;

class BaseEnemyMovementComponent : public BaseComponent //TODO: rename this class to behavior or something, we are not talking only about movement.
	//Also teach it maybe to shoot from here. And Boolean if is dead or alive.
{
public:
	BaseEnemyMovementComponent(float speed, int birdCompanionIndex = -1, glm::vec2 posInFormation = glm::vec2{ 200,200 });//TODO: get rid of these nasty numbers after all the tests
	virtual ~BaseEnemyMovementComponent();
	float GetSpeed();
	glm::vec2 GetPosInFormation();
	virtual void Update();
	void Switch();
	void ShootARocket();
	void Die();//TODO: make unique score++ for each state's Die

	bool GetIsInFormation() const;
	void SetIsInFormation(bool inFormation);
	int GetBirdCompanionIndex() { return m_BirdCompanionIndex; }

protected:
	BaseEnemyState* m_CurrentState;
	glm::vec2 m_PosInFormation;
	float m_Speed;
	bool m_InFormation;

	int m_BirdCompanionIndex;

	//TODO: Mby make getters and setters for these datamembers, this will be a good opportunity to make an adjustable difficulty
	//float m_StepTimer = 0;
	//float m_StepTime;
	//int m_StepsNumber;
	//int m_CurrentStepNumber = 0;
	//int m_StepSize;
	//bool m_MovingLeft = true;
};

