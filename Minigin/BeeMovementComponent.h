#pragma once
#include "BaseComponent.h"
#include "BaseEnemyMovementComponent.h"
#include "BaseEnemyState.h"

class BeeMovementComponent final : public BaseEnemyMovementComponent
{
public:
	BeeMovementComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });
	void Update() override;
};

