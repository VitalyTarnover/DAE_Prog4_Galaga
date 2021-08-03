#include "MiniginPCH.h"
#include "BeeMovementComponent.h"
#include "BeeFlyInState.h"

BeeMovementComponent::BeeMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, -1,  posInFormation)//Bees can't be bird's companions
{
	m_CurrentState = new BeeFlyInState(m_Speed);
}


