#pragma once
#include "Singleton.h"

enum class Level
{
	Level1,
	Level2,
	Level3
};

class LevelManager : public dae::Singleton<LevelManager>
{
	Level GetCurrentLevel() const { return m_CurrentLevel; };
	void SetLevel(int level) { m_CurrentLevel = Level(level); };
private:
	Level m_CurrentLevel = Level::Level1;
};

