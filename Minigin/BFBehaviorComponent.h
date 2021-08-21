#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"


class BFBehaviorComponent : public BaseEnemyBehaviorComponent
{
public:
	BFBehaviorComponent(float speed, int birdCompanionIndex, const glm::vec2& posInFormation);

	bool GetIsWithBird() const { return m_IsWithBird; }
	void SetIsWithBird(bool isWithBird) { m_IsWithBird = isWithBird; }
private:
	void Die(const std::shared_ptr<GameObject>& killerObject) const override;

	bool m_IsWithBird = false;
};

