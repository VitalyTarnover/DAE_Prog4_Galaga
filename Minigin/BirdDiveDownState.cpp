#include "MiniginPCH.h"
#include "BirdDiveDownState.h"

BirdDiveDownState::BirdDiveDownState(float speed)
	:m_Speed{speed}
	,m_CurrentWaypoint{0}
{
}

BaseEnemyState* BirdDiveDownState::Update(GameObject* enemy)
{
	UNREFERENCED_PARAMETER(enemy);
	return nullptr;
}

void BirdDiveDownState::CreatePaths(GameObject* enemy)
{
	UNREFERENCED_PARAMETER(enemy);

}

bool BirdDiveDownState::BirdDiveDown(GameObject* enemy)
{
	UNREFERENCED_PARAMETER(enemy);

	return false;
}
