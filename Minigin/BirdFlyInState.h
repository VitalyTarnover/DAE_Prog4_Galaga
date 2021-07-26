#pragma once
#include "BaseEnemyState.h"

class BirdFlyInState : public BaseEnemyState
{
public:
	BirdFlyInState(float speed, int stepSize = 5);
	virtual BaseEnemyState* Update(GameObject* enemy) override;
private:
	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	void CreatePaths(GameObject* enemy);
	void BirdFlyIn(GameObject* enemy);

	int m_StepSize;//for patrolling before formation
};

