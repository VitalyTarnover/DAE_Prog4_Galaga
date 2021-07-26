#pragma once
#include "BaseEnemyState.h"

class BFFlyInState : public BaseEnemyState
{
public:
	BFFlyInState(float speed, int stepSize = 5);
	virtual BaseEnemyState* Update(GameObject* enemy) override;
private:
	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	void CreatePaths(GameObject* enemy);
	void BFFlyIn(GameObject* enemy);

	int m_StepSize;//for patrolling before formation

	//for observer pattern
	//Event m_EnemyReachedPosInFormation;
};

