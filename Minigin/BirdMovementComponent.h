#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

class BirdMovementComponent : public BaseEnemyMovementComponent
{
public:
	BirdMovementComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });
};
