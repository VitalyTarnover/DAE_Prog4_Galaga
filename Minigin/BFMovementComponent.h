#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"


class BFMovementComponent : public BaseEnemyMovementComponent
{
public:
	BFMovementComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });
};

