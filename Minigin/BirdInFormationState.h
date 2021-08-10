#pragma once
#include "InFormationState.h"

class BirdInFormationState final : public InFormationState
{
public:
	BirdInFormationState();
	BaseEnemyState* Update(GameObject* enemy) override;
private:
	void SetSpriteState(GameObject* enemy) override;
};

