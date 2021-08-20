#include "MiniginPCH.h"
#include "BFBehaviorComponent.h"
#include "BFFlyInState.h"

BFBehaviorComponent::BFBehaviorComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:BaseEnemyBehaviorComponent(speed, birdCompanionIndex, posInFormation)
{
	m_CurrentState = new BFFlyInState(m_Speed);
}

void BFBehaviorComponent::Die(std::shared_ptr<GameObject> killerObject)
{
	BaseEnemyBehaviorComponent::Die(killerObject);
	if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBFKilled");
	else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BFKilled");
}


