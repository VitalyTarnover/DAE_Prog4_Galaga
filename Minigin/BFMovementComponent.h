#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"


class BFMovementComponent : public BaseComponent
{
public:
	BFMovementComponent();
	void Update() override;
};

