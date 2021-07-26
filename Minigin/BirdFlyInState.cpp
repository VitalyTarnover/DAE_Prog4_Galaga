#include "MiniginPCH.h"
#include "BirdFlyInState.h"

BirdFlyInState::BirdFlyInState(float speed, int stepSize)
	:BaseEnemyState()
	, m_CurrentWaypoint{ 0 }
	, m_Speed{ speed }
	, m_StepSize{ stepSize }
{
}

BaseEnemyState* BirdFlyInState::Update(GameObject* enemy)
{
    return nullptr;
}

void BirdFlyInState::CreatePaths(GameObject* enemy)
{
}

void BirdFlyInState::BirdFlyIn(GameObject* enemy)
{
}
