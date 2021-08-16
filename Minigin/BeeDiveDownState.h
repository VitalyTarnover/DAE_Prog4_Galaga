#pragma once
#include "BaseEnemyState.h"

class BeeDiveDownState : public BaseEnemyState
{
public:
	BeeDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy);
	bool BeeDiveDown(GameObject* enemy);

	std::vector<glm::vec2> m_Path;
	float m_Speed;
	int m_CurrentWaypoint;
	glm::vec2 m_Direction;
	float m_SqrMagnitude = FLT_MAX;
};

