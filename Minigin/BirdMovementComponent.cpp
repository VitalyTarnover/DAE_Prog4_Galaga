#include "MiniginPCH.h"
#include "BirdMovementComponent.h"
#include "BirdFlyInState.h"

BirdMovementComponent::BirdMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, posInFormation)
{
	m_CurrentState = new BirdFlyInState(m_Speed);
}


