#pragma once
#include "BaseComponent.h"	
class ScoreComponent final : public BaseComponent
{
public:
	ScoreComponent() {};

	int GetScore() const { return m_Score; };
	void SetScore(int score) { m_Score = score; };
	void IncreaseScore(int score) { m_Score += score; };
private:
	int m_Score = 0;
};

