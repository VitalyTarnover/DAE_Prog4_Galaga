#include "MiniginPCH.h"
#include "BeeMovementComponent.h"
#include "BeeFlyInState.h"

BeeMovementComponent::BeeMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, posInFormation)
{
	m_CurrentState = new BeeFlyInState(m_Speed);
}


