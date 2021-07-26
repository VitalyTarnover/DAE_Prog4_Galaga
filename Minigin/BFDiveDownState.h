#pragma once
#include "BaseEnemyState.h"

class BFDiveDownState : public BaseEnemyState
{
public:
	BFDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy);
	bool BFDiveDown(GameObject* enemy);

	std::vector<glm::vec2> m_Path;
	float m_Speed;
	int m_CurrentWaypoint;
};

