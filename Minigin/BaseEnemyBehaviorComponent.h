#pragma once
#include "BaseComponent.h"
#include "Event.h"

//states: fly in, in formation, dive down, die
class BaseEnemyState;
class Event;

class BaseEnemyBehaviorComponent : public BaseComponent
{
public:
	BaseEnemyBehaviorComponent(float speed, int birdCompanionIndex = -1, glm::vec2 posInFormation = glm::vec2{});
	virtual ~BaseEnemyBehaviorComponent();
	glm::vec2 GetPosInFormation();
	virtual void Update();
	void Switch();
	void SetPanic(bool panic) { m_Panic = panic; };
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

	std::shared_ptr<Event> m_pEventEnemyKilled;

};

