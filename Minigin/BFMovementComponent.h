#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"


class BFMovementComponent : public BaseEnemyMovementComponent
{
public:
	BFMovementComponent(float speed, int birdCompanionIndex = -1, glm::vec2 posInFormation = glm::vec2{ 200,200 });
};

