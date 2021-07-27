#pragma once
#include "BaseComponent.h"

class PlayerHealthComponent final : public BaseComponent //TODO: check if everything is final, const ref and stuff
{
public:
	PlayerHealthComponent(int lives);

	void Update() override;
	int GetLives() const;
	void SetLives(int);
	void Die();
	void Respawn();
	bool IsAlive() const;
private:
	float m_RespawnTimer;
	float m_RespawnTime;

	int m_Lives;
	bool m_Dead;

};

