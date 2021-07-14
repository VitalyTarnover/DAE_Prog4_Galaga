#include "MiniginPCH.h"
#include "BeeMovementComponent.h"
#include "BeeFlyInState.h"

BeeMovementComponent::BeeMovementComponent()
{
	m_CurrentState = new BeeFlyInState();
}

void BeeMovementComponent::Update()
{
	BaseEnemyState* state = m_CurrentState->Update(*this);
	if (state != NULL)
	{
		delete state_;
		state_ = state;
	}
}
