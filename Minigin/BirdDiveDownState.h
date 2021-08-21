#pragma once
#include "BaseDynamicState.h"

enum class TractorBeamStage
{
	Stage0,
	Stage1,
	Stage2,
	Stage3
};

class BirdDiveDownState final : public BaseDynamicState 
{
public:
	BirdDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void SetSpriteState(GameObject* enemy);

	void TractorBeamAttack(GameObject* enemy);
	void CreatePaths(GameObject* enemy) override;

	bool BirdDiveDown(GameObject* enemy);

	bool m_BombingAttack;
	bool m_TractorBeamActivated = false;

	bool m_SpriteOffset = false;

	float m_TractorBeamTimer = 0.0f;
	float m_TractorBeamTime = 6.0f;

	TractorBeamStage m_TractorBeamStage = TractorBeamStage::Stage0;


};

