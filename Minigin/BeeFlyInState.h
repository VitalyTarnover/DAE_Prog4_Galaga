#pragma once
#include "BaseEnemyState.h"

class BeeFlyInState final : public BaseEnemyState
{
public:
	BeeFlyInState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	void CreatePaths(GameObject* enemy);
	void BeeFlyIn(GameObject* enemy);

};

