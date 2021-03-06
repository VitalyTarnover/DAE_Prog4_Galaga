#pragma once
#include "Singleton.h"
#include "glm/vec3.hpp"
#include "Event.h"

class RocketManager final : public dae::Singleton<RocketManager>
{
public:

	void ReduceActiveRocketsNumber();
	void ReduceActiveEnemyRocketsNumber();
	void SpawnPlayerRocket(int playerIndex);
	void SpawnEnemyRocket(const glm::vec3& enemyPos) const;
	void SpawnEnemyRocket();

	void ShotHit();
	int GetNumberOfShotsFired() const { return m_NumberOfShotsFired; };
	int GetNumberOfHits() const { return m_NumberOfHits; };
	void Reset();

	void SetAudioEventHandler(const std::shared_ptr<IEventHandler>& audioEventHandler);
private:
	Event m_EventShotFired;

	int m_ActiveRocketsNumber = 0;
	int m_AllowedRocketsNumber = 2;

	int m_ActiveEnemyRocketsNumber = 0;
	int m_AllowedEnemyRocketsNumber = 2;

	int m_NumberOfShotsFired = 0;
	int m_NumberOfHits = 0;
};

