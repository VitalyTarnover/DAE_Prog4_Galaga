#pragma once
#include "BaseComponent.h"
#include "Event.h"

//states: fly in, in formation, dive down, die
class BaseEnemyState;
class Event;

class BaseEnemyMovementComponent : public BaseComponent //TODO: rename this class to behavior or something, we are not talking only about movement.
	//Also teach it maybe to shoot from here. And Boolean if is dead or alive.
{
public:
	BaseEnemyMovementComponent(float speed, int birdCompanionIndex = -1, glm::vec2 posInFormation = glm::vec2{ 200,200 });//TODO: get rid of these nasty numbers after all the tests
	virtual ~BaseEnemyMovementComponent();
	float GetSpeed() const;//TODO: is it required?
	glm::vec2 GetPosInFormation();
	virtual void Update();
	void Switch();
	void SetPanic() { m_Panic = true; };
	bool GetIsPanicing() const { return m_Panic; };
	void ShootARocket();
	virtual void Die(std::shared_ptr<GameObject> killerObject);

	bool GetIsInFormation() const;
	void SetIsInFormation(bool inFormation);
	
	int GetBirdCompanionIndex() { return m_BirdCompanionIndex; }


	BaseEnemyState* GetCurrentState() const { return m_CurrentState; };

	std::shared_ptr<Event> GetEventEnemyKilledHandler() const { return m_pEventEnemyKilled; };

	void SetIsAttacking(bool isAttacking) { m_IsAttacking = isAttacking; };
	bool GetIsAttacking() { return m_IsAttacking; };

protected:
	BaseEnemyState* m_CurrentState;
	glm::vec2 m_PosInFormation;
	float m_Speed;
	bool m_InFormation;
	bool m_IsAttacking = false;
	bool m_Panic = false;

	int m_BirdCompanionIndex;

	std::shared_ptr<Event> m_pEventEnemyKilled;//It works well if it is a shared ptr

	//TODO: Mby make getters and setters for these datamembers, this will be a good opportunity to make an adjustable difficulty
	//float m_StepTimer = 0;
	//float m_StepTime;
	//int m_StepsNumber;
	//int m_CurrentStepNumber = 0;
	//int m_StepSize;
	//bool m_MovingLeft = true;
};

