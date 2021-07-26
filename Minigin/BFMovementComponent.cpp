#include "MiniginPCH.h"
#include "BFMovementComponent.h"
#include "BFFlyInState.h"

BFMovementComponent::BFMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, posInFormation)
{
	m_CurrentState = new BFFlyInState(m_Speed);
}

void BFMovementComponent::Update()
{
	BaseEnemyState* state = m_CurrentState->Update(m_pGameObject);
	if (state != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = state;
	}
}
