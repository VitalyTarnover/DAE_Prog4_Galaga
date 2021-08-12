#include "MiniginPCH.h"
#include "BeeMovementComponent.h"
#include "BeeFlyInState.h"

void BeeMovementComponent::Die(std::shared_ptr<GameObject> killerObject)
{
	BaseEnemyMovementComponent::Die(killerObject);
	if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBeeKilled");
	else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BeeKilled");
}

BeeMovementComponent::BeeMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, -1,  posInFormation)//Bees can't be bird's companions
{
	m_CurrentState = new BeeFlyInState(m_Speed);
}


