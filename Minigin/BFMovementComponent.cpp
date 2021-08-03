#include "MiniginPCH.h"
#include "BFMovementComponent.h"
#include "BFFlyInState.h"

BFMovementComponent::BFMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, birdCompanionIndex, posInFormation)
{
	m_CurrentState = new BFFlyInState(m_Speed);
}


