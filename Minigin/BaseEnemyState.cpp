#include "MiniginPCH.h"
#include "BaseEnemyState.h"

BaseEnemyState::BaseEnemyState(GameObject* enemy)
	: m_Enemy{enemy}
	, m_Switch{ false }
{
}

void BaseEnemyState::Switch()
{
	m_Switch = true;
}
