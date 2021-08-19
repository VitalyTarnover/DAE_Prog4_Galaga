#pragma once
#include "Singleton.h"
#include "glm/vec3.hpp"

class RocketManager final : public dae::Singleton<RocketManager>
{
public:

	void ReduceActiveRocketsNumber();
	void ReduceActiveEnemyRocketsNumber();
	void SpawnPlayerRocket(int playerIndex);
	void SpawnEnemyRocket(const glm::vec3& enemyPos);
	void SpawnEnemyRocket();

	void ShotHit();
	int GetNumberOfShotsFired() const { return m_NumberOfShotsFired; };
	int GetNumberOfHits() const { return m_NumberOfHits; };
	void ResetStatistics();
private:
	int m_ActiveRocketsNumber = 0;
	int m_AllowedRocketsNumber = 20;

	int m_ActiveEnemyRocketsNumber = 0;
	int m_AllowedEnemyRocketsNumber = 2;

	int m_NumberOfShotsFired = 0;
	int m_NumberOfHits = 0;
};

