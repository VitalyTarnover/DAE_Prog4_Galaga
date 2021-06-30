#pragma once
#include "BaseComponent.h"

class ScoreComponent final : public BaseComponent
{
public:
	ScoreComponent(const unsigned int& score);

	const unsigned int& GetScore() const { return m_Score; };
	void SetScore(const unsigned int&);
	void IncreaseScore(const int score);
private:
	unsigned int m_Score;
};
