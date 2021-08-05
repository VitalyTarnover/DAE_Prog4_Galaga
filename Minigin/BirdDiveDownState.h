#pragma once
#include "BaseEnemyState.h"

class BirdDiveDownState : public BaseEnemyState
{
public:
	BirdDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void ActivateTractorBeam();
	void CreatePaths(GameObject* enemy);
	bool BirdDiveDown(GameObject* enemy);

	std::vector<glm::vec2> m_Path;
	float m_Speed;
	int m_CurrentWaypoint;

	bool m_BombingAttack;
	bool m_TractorBeamActivated = false;

	float m_TractorBeamTimer = 0.0f;
	float m_TractorBeamTime = 6.0f;

};

