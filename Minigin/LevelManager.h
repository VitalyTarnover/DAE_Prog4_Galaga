#pragma once
#include "Singleton.h"

class LevelManager : public dae::Singleton<LevelManager>
{
public:
	int GetCurrentLevel() const { return m_CurrentLevel; };
	void NextLevel();
	void ResetLevel() { m_CurrentLevel = 0; };
private:
	int m_CurrentLevel = 0;
};

