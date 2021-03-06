#pragma once
#include "BaseComponent.h"

class FighterShipMovementComponent final : public BaseComponent
{
public:
	FighterShipMovementComponent(float speed = 50);

	void StartMoving(bool left);
	void StopMoving(bool left);

	void Update() override;

	void SetMovementLocked(bool locked);
private:
	bool m_MovementLocked;
	void Move() const;
	float m_Speed;

	bool m_MovingLeft;
	bool m_MovingRight;

};

