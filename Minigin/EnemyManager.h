#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"


//states: building formation, battle

class GameObject;

class EnemyManager final : public dae::Singleton<EnemyManager>
{
public:
	~EnemyManager();
	void SpawnEnemies(const std::vector<glm::vec2>& posInFormation);
	void Update();
	void ResetEnemies();
	void AnEnemyReachedPositionInFormation();
private:
	float m_SpawnTimer = 0;
	float m_SpawnTime = 0.5f;

	bool m_BuildingFormation;//if not then it is battle stage

	std::vector<glm::vec2> m_BeesPosInFormation;

	std::vector<std::shared_ptr<GameObject>> m_Enemies;

	int m_NumberOfEnemiesNotInPosition;
	int m_NumberOfEnemiesAlive;

};

