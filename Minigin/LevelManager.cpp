#include "MiniginPCH.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "GalagaFileReader.h"
#include "ScoreEventHandler.h"
#include "LevelClearedEventHandler.h"

void LevelManager::NextLevel()
{
	if (m_CurrentLevel < 3)
	{
		++m_CurrentLevel;
		//TODO: manager resets


		std::shared_ptr<ScoreEventHandler> scoreEventHandler = std::make_shared<ScoreEventHandler>();
		std::shared_ptr<LevelClearedEventHandler> levelClearedEventHandler = std::make_shared<LevelClearedEventHandler>();

		std::vector<std::shared_ptr<IEventHandler>> handlers{};
		handlers.push_back(scoreEventHandler);
		handlers.push_back(levelClearedEventHandler);

		GalagaFileReader* gfr = new GalagaFileReader();
		gfr->ReadLevelInfo("Resources/Level" + std::to_string(m_CurrentLevel) + ".bin");

		EnemyManager::GetInstance().SpawnEnemies(gfr->GetBeeInfo(), gfr->GetBFInfo(), gfr->GetBirdInfo(), handlers);

		delete gfr;
	}
}
