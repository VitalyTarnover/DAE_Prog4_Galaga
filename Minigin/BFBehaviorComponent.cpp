#include "MiniginPCH.h"
#include "BFBehaviorComponent.h"
#include "BFFlyInState.h"

BFBehaviorComponent::BFBehaviorComponent(float speed, int birdCompanionIndex,const glm::vec2& posInFormation)
	:BaseEnemyBehaviorComponent(speed, birdCompanionIndex, posInFormation)
{
	m_pCurrentState = new BFFlyInState(m_Speed);
}

void BFBehaviorComponent::Die(const std::shared_ptr<GameObject>& killerObject) const
{
	BaseEnemyBehaviorComponent::Die(killerObject);
	if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBFKilled");
	else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BFKilled");
}


