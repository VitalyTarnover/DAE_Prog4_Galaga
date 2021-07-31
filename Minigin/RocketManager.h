#pragma once
#include "Singleton.h"
#include "glm/vec3.hpp"

class RocketManager final : public dae::Singleton<RocketManager>
{
public:

	void ReduceActiveRocketsNumber();
	void ReduceActiveEnemyRocketsNumber();
	void SpawnPlayerRocket();
	void SpawnEnemyRocket(const glm::vec3& enemyPos);

private:
	int m_ActiveRocketsNumber = 0;
	int m_ActiveEnemyRocketsNumber = 0;
	int m_AllowedRocketsNumber = 20;
	int m_AllowedEnemyRocketsNumber = 1;
};

