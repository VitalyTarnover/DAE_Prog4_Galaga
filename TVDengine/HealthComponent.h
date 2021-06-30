#pragma once
#include "BaseComponent.h"

class HealthComponent final : public BaseComponent
{
public:
	HealthComponent(const unsigned int& health);

	const unsigned int& GetLives() const { return m_Lives; };
	void SetLives(const unsigned int&);
	void Die();
	void Respawn();
private:
	unsigned int m_Lives;
	bool m_Dead;
};