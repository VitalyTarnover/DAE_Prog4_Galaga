#pragma once
#include "BaseComponent.h"


class RocketMovementComponent final : public BaseComponent
{
public:
	RocketMovementComponent(bool movesUp = true, float speed = 250); //movesUp also means it was shot by a player

	void Update() override;

	bool GetMovesUp() { return m_MovesUp; }

private:
	void Move();
	void OutsideBordersCheck();

	float m_Speed;

	bool m_MovesUp;
};

