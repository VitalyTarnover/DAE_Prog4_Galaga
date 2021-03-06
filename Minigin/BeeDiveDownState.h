#pragma once
#include "BaseDynamicState.h"

class BeeDiveDownState final: public BaseDynamicState
{
public:
	BeeDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy) override;
	bool BeeDiveDown(GameObject* enemy);

};

