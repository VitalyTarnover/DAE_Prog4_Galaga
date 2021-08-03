#include "MiniginPCH.h"
#include "BirdMovementComponent.h"
#include "BirdFlyInState.h"

BirdMovementComponent::BirdMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, birdCompanionIndex, posInFormation)
{
	m_CurrentState = new BirdFlyInState(m_Speed);
}


