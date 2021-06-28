#pragma once
#include "Components.h"


class FighterShipMovementComponent final : public BaseComponent
{
public:
	FighterShipMovementComponent(float speed = 50);

	void StartMoving(bool left);
	void StopMoving(bool left);

	void Update();


private:
	void Move();
	float m_Speed;

	bool m_MovingLeft;
	bool m_MovingRight;

};

