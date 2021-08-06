#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

class BirdMovementComponent : public BaseEnemyMovementComponent
{
public:
	BirdMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation = glm::vec2{ 200,200 });
	
	bool GetIsBombing() const { return m_BombingAttack; }//TODO: check if all getters are const, you might have forgotten
	void SetIsBombing(bool isBombing) { m_BombingAttack = isBombing;  }

	void FighterCaptured() { m_HasFighterCaptured = true; };
private:
	bool m_BombingAttack = false;//so by default if it is picked it will tractor beam attack
	bool m_HasFighterCaptured = false;
};
