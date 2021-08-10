#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

class BirdMovementComponent final: public BaseEnemyMovementComponent
{
public:
	BirdMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation = glm::vec2{ 200,200 });
	~BirdMovementComponent();

	void Update() override;

	bool GetIsBombing() const { return m_BombingAttack; }//TODO: check if all getters are const, you might have forgotten
	void SetIsBombing(bool isBombing) { m_BombingAttack = isBombing;  }

	void FighterCaptured() { m_HasFighterCaptured = true; };

	void HandleCapturedFighter();

	bool GetHasFighterCaptured() const { return m_HasFighterCaptured; };

	bool GetIsHurt() const { return m_IsHurt; };
	void Hurt();
private:

	bool m_IsHurt = false;

	bool m_BombingAttack = false;//so by default if it is picked it will tractor beam attack
	bool m_HasFighterCaptured = false;

	float m_CapturedFighterShootingTime = 3.f;
	float m_CapturedFighterShootingTimer = 0.f;

	std::shared_ptr<GameObject> m_pCapturedFighter = nullptr;

};
