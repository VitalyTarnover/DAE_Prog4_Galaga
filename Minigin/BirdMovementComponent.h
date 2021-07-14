#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

class BirdMovementComponent : public BaseComponent
{
public:
	BirdMovementComponent();
	void Update() override;
};
