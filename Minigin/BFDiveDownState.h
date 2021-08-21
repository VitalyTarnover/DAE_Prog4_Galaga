#pragma once
#include "BaseDynamicState.h"

class BFDiveDownState final : public BaseDynamicState
{
public:
	BFDiveDownState(float speed);
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void CreatePaths(GameObject* enemy) override;
	bool BFDiveDown(GameObject* enemy);
};

