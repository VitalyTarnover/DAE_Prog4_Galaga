#pragma once
#include "BaseComponent.h"
#include "BaseEnemyBehaviorComponent.h"
#include "BaseEnemyState.h"

class BeeBehaviorComponent final : public BaseEnemyBehaviorComponent
{
	void Die(const std::shared_ptr<GameObject>& killerObject) const override;
public:
	BeeBehaviorComponent(float speed, const glm::vec2& posInFormation );
};

