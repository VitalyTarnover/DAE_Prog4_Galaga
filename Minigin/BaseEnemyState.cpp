#include "MiniginPCH.h"
#include "BaseEnemyState.h"

BaseEnemyState::BaseEnemyState()
	: m_Switch{ false }
{
}

void BaseEnemyState::Switch()
{
	m_Switch = true;
}
