#pragma once
#include "BaseComponent.h"
#include "BaseEnemyMovementComponent.h"
#include "BaseEnemyState.h"

class BeeMovementComponent final : public BaseEnemyMovementComponent
{
public:
	BeeMovementComponent();
	void Update() override;
};

