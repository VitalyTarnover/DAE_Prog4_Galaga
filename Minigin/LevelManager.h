#pragma once
#include "Singleton.h"
#include "GameObject.h"


class LevelManager : public dae::Singleton<LevelManager>
{
public:
	void Update();
	int GetCurrentLevel() const { return m_CurrentLevel; };
	void NextLevel();
	void ResetLevelManager();
	void CheckEndGameConditions(GameObject* player);
private:
	void EndGameSequence();

	float m_ResultsTimer = 0.f;
	float m_ResultsTime = 4.f;

	int m_CurrentLevel = 0;
	
	bool m_Player1Lost = false;
	bool m_Player2Lost = false;

	bool m_EndGame = false;


};

