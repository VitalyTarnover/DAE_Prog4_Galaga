#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"


class BFMovementComponent : public BaseEnemyMovementComponent
{
public:
	BFMovementComponent(float speed, int birdCompanionIndex = -1, glm::vec2 posInFormation = glm::vec2{ 200,200 });

	bool GetIsWithBird() const { return m_IsWithBird; }
	void SetIsWithBird(bool isWithBird) { m_IsWithBird = isWithBird; }
private:
	void Die(std::shared_ptr<GameObject> killerObject) override;

	bool m_IsWithBird = false;

};

