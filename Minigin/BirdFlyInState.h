#pragma once
#include "BaseDynamicState.h"

class BirdFlyInState final : public BaseDynamicState
{
public:
	BirdFlyInState(float speed, int stepSize = 5);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy) override;
	void Move(GameObject* enemy) override;

	void SetSpriteState(GameObject* enemy);

	bool m_SpriteOffset = false;
};

