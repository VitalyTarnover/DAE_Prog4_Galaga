#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"


//states: building formation, battle

class EnemyManager final : public dae::Singleton<EnemyManager>
{
public:
	void SpawnEnemies(const std::vector<glm::vec2>& posInFormation);
	void Update();
private:
	float m_SpawnTimer = 0;
	float m_SpawnTime = 0.5f;

	std::vector<glm::vec2> m_BeesPosInFormation;
	int m_NumberOfEnemiesNotInPosition;
	int m_NumberOfEnemiesAlive;

};

