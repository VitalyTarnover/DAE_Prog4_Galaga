#pragma once
#include "BaseComponent.h"


class RocketMovementComponent final : public BaseComponent
{
public:
	RocketMovementComponent(bool movesUp = true, float speed = 250); //movesUp also means it was shot by a player

	void Update() override;

	bool GetMovesUp() { return m_MovesUp; }

	std::shared_ptr<GameObject> GetOwner() const { return m_pOwner; };
	void SetOwner(std::shared_ptr<GameObject> newOwner) { m_pOwner = newOwner; };

private:
	void Move();
	void OutsideBordersCheck();

	float m_Speed;

	bool m_MovesUp;

	glm::vec2 m_Direction;

	std::shared_ptr<GameObject> m_pOwner = nullptr;
};

