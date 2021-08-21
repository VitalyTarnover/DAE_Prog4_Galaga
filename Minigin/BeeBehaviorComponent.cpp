#include "MiniginPCH.h"
#include "BeeBehaviorComponent.h"
#include "BeeFlyInState.h"

void BeeBehaviorComponent::Die(const std::shared_ptr<GameObject>& killerObject) const
{
	BaseEnemyBehaviorComponent::Die(killerObject);
	if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBeeKilled");
	else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BeeKilled");
}

BeeBehaviorComponent::BeeBehaviorComponent(float speed, const glm::vec2& posInFormation)
	:BaseEnemyBehaviorComponent(speed, -1,  posInFormation)//Bees can't be bird's companions
{
	m_pCurrentState = new BeeFlyInState(m_Speed);
}


