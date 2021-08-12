#include "MiniginPCH.h"
#include "BFMovementComponent.h"
#include "BFFlyInState.h"

BFMovementComponent::BFMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, birdCompanionIndex, posInFormation)
{
	m_CurrentState = new BFFlyInState(m_Speed);
}

void BFMovementComponent::Die(std::shared_ptr<GameObject> killerObject)
{
	BaseEnemyMovementComponent::Die(killerObject);
	if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBFKilled");
	else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BFKilled");
}


