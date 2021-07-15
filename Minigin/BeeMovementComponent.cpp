#include "MiniginPCH.h"
#include "BeeMovementComponent.h"
#include "BeeFlyInState.h"

BeeMovementComponent::BeeMovementComponent(float speed, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, posInFormation)
{
	m_CurrentState = new BeeFlyInState();
}

void BeeMovementComponent::Update()
{
	BaseEnemyState* state = m_CurrentState->Update(m_pGameObject);
	if (state != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = state;
	}
}
