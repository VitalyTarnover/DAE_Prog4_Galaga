#pragma once
#include "Singleton.h"
#include "glm/vec3.hpp"

class GameObject;

class ExplosionManager final : public dae::Singleton<ExplosionManager>
{
public:
	void MakeExplosion(const glm::vec3& position);
	void Update();
private:
	void ExplosionTimersHandler();

	std::vector<std::pair<std::shared_ptr<GameObject>, float>> m_ExplosionsAndTimers;
	float m_ExplosionTime = 0.25f;

};

