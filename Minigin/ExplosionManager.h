#pragma once
#include "Singleton.h"
#include "glm/vec3.hpp"

class ExplosionManager : public dae::Singleton<ExplosionManager>
{
public:

	void MakeExplosion(glm::vec3 position);

	void Update();


private:
	void ExplosionTimersHandler();

	std::vector<float> m_ExplosionTimers;
	float m_ExplosionTime = 0.5f;

};

