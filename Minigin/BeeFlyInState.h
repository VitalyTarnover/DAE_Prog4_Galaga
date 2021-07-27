#pragma once
#include "BaseEnemyState.h"
#include "Event.h"

class BeeFlyInState final : public BaseEnemyState
{
public:
	BeeFlyInState(float speed, int stepSize = 5);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	void CreatePaths(GameObject* enemy);
	void BeeFlyIn(GameObject* enemy);

	int m_StepSize;//for patrolling before formation

	//for observer pattern V@
	//Event m_EnemyReachedPosInFormation;

};

