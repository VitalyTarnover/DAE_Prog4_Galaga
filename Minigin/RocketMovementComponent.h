#pragma once
#include "BaseComponent.h"


class RocketMovementComponent final : public BaseComponent
{
public:
	RocketMovementComponent(bool movesUp = true, float speed = 250, bool enemyPlayerShot = false); //movesUp also means it was shot by a player
	~RocketMovementComponent();
	void Update() override;

	bool GetMovesUp() const { return m_MovesUp; }

	std::shared_ptr<GameObject> GetOwner() const { return m_pOwner; };
	void SetOwner(const std::shared_ptr<GameObject>& newOwner) { m_pOwner = newOwner; };

private:
	void Move() const;
	void OutsideBordersCheck() const;

	float m_Speed;

	bool m_MovesUp;

	bool m_EnemyPlayerShot;

	glm::vec2 m_Direction;

	std::shared_ptr<GameObject> m_pOwner = nullptr;
};

