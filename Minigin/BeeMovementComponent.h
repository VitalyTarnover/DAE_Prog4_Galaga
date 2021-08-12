#pragma once
#include "BaseComponent.h"
#include "BaseEnemyMovementComponent.h"
#include "BaseEnemyState.h"

class BeeMovementComponent final : public BaseEnemyMovementComponent
{
	void Die(std::shared_ptr<GameObject> killerObject) override;
public:
	BeeMovementComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });
};

