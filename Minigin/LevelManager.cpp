#include "MiniginPCH.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "GalagaFileReader.h"
#include "ScoreEventHandler.h"
#include "LevelClearedEventHandler.h"
#include "SceneLoader.h"

void LevelManager::Update()
{
	if (m_EndGame)EndGameSequence();
}

void LevelManager::NextLevel()
{
	if (m_CurrentLevel < 3 )
	{
		++m_CurrentLevel;
		
		GalagaFileReader* gfr = new GalagaFileReader();
		gfr->ReadLevelInfo("Resources/Level" + std::to_string(m_CurrentLevel) + ".bin");

		EnemyManager::GetInstance().SpawnEnemies(gfr->GetBeeInfo(), gfr->GetBFInfo(), gfr->GetBirdInfo(), SceneLoader::GetInstance().GetEventhandlers());

		delete gfr;
	}
	else
	{
		if (!m_EndGame)
		{
			m_EndGame = true;
			SceneLoader::GetInstance().ShowResultsScreen();
		}
	}
}

void LevelManager::ResetLevelManager()
{
	m_ResultsTimer = 0.f;
	m_CurrentLevel = 0;

	m_Player1Lost = false;
	m_Player2Lost = false;

	m_EndGame = false;
}

void LevelManager::CheckEndGameConditions(GameObject* player)
{
	if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::Coop || player->GetName() == "Player2")
		m_Player2Lost = true;

	if (player->GetName() == "Player1") m_Player1Lost = true;


	if (m_Player1Lost && m_Player2Lost && m_EndGame == false)
	{
		m_EndGame = true;
		SceneLoader::GetInstance().ShowResultsScreen();
	}

}

void LevelManager::EndGameSequence()
{
	if (m_ResultsTimer < m_ResultsTime)	m_ResultsTimer += SystemTime::GetInstance().GetDeltaTime();
	else SceneLoader::GetInstance().LoadMainMenu();
}


