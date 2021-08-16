#pragma once
#include "BaseDynamicState.h"

class BFFlyInState final : public BaseDynamicState
{
public:
	BFFlyInState(float speed, int stepSize = 5);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy) override;
	void Move(GameObject* enemy) override;
};

