#include "MiniginPCH.h"
#include "LevelClearedEventHandler.h"
#include "LevelManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextComponent.h"

void LevelClearedEventHandler::OnEvent(GameObject*, const std::string& argument)
{
	if (argument == "LevelCleared")
	{
		LevelManager::GetInstance().NextLevel();
		UpdateHUD();
	}
}

void LevelClearedEventHandler::UpdateHUD() const
{
	std::shared_ptr<GameObject> levelDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("LevelDisplay");
	levelDisplay->GetComponent<TextComponent>()->UpdateText("Level: " + std::to_string(LevelManager::GetInstance().GetCurrentLevel()));
}
