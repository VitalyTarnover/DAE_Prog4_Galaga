#pragma once
#include "BaseEnemyState.h"

enum class TractorBeamStage
{
	Stage0,
	Stage1,
	Stage2,
	Stage3
};

class BirdDiveDownState : public BaseEnemyState
{
public:
	BirdDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void TractorBeamAttack(GameObject* enemy);
	void CreatePaths(GameObject* enemy);
	bool BirdDiveDown(GameObject* enemy);

	std::vector<glm::vec2> m_Path;
	float m_Speed;
	int m_CurrentWaypoint;

	bool m_BombingAttack;
	bool m_TractorBeamActivated = false;

	float m_TractorBeamTimer = 0.0f;
	float m_TractorBeamTime = 6.0f;

	TractorBeamStage m_TractorBeamStage = TractorBeamStage::Stage0;
};

