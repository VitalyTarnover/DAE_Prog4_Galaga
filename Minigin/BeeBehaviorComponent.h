#pragma once
#include "BaseComponent.h"
#include "BaseEnemyBehaviorComponent.h"
#include "BaseEnemyState.h"

class BeeBehaviorComponent final : public BaseEnemyBehaviorComponent
{
	void Die(std::shared_ptr<GameObject> killerObject) override;
public:
	BeeBehaviorComponent(float speed, glm::vec2 posInFormation = glm::vec2{ 200,200 });
};

