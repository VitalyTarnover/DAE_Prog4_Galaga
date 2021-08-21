#include "MiniginPCH.h"
#include "BaseEnemyState.h"
#include "GameObject.h"

BaseEnemyState::BaseEnemyState()
	: m_Switch{ false }
{
}

void BaseEnemyState::Switch()
{
	m_Switch = true;
}

void BaseEnemyState::Die(GameObject* enemy) const
{
	enemy->SetMarkedForDelete(true);
	enemy = nullptr;
}
