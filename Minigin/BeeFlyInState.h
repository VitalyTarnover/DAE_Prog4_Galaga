#pragma once
#include "BaseEnemyState.h"

class BeeFlyInState final : public BaseEnemyState
{
public:
	BeeFlyInState(GameObject* enemy, float speed);
	BaseEnemyState* Update() override;
private:
	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	void CreatePaths();
	void BeeFlyIn();

};

