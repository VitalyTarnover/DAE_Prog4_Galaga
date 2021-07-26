#include "MiniginPCH.h"
#include "BFMovementComponent.h"
#include "BFFlyInState.h"

BFMovementComponent::BFMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, posInFormation)
{
	m_CurrentState = new BFFlyInState(m_Speed);
}


