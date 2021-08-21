#pragma once
#include "BaseComponent.h"
#include "BaseEnemyState.h"

class BirdBehaviorComponent final: public BaseEnemyBehaviorComponent
{
public:
	BirdBehaviorComponent(float speed, int birdCompanionIndex,const glm::vec2& posInFormation, bool playerControlled = false);
	~BirdBehaviorComponent();

	void Update() override;

	bool GetIsBombing() const { return m_BombingAttack; };
	void SetIsBombing(bool isBombing) { m_BombingAttack = isBombing; };

	void FighterCaptured() { m_HasFighterCaptured = true; };

	void HandleCapturedFighter();

	bool GetHasFighterCaptured() const { return m_HasFighterCaptured; };

	bool GetIsHurt() const { return m_IsHurt; };
	void Hurt(const std::shared_ptr<GameObject>& killerObject);
	
	bool GetIsControlledByPlayer() const { return m_IsPlayerControlled; };

private:
	void Die(const std::shared_ptr<GameObject>& killerObject) const override;

	bool m_IsHurt = false;
	bool m_IsPlayerControlled = false;

	bool m_BombingAttack = false;
	bool m_HasFighterCaptured = false;

	float m_CapturedFighterShootingTime = 3.f;
	float m_CapturedFighterShootingTimer = 0.f;

	std::shared_ptr<GameObject> m_pCapturedFighter = nullptr;

};
