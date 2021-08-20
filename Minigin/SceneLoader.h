#pragma once
#include "Singleton.h"

enum class GameMode
{
	MainMenu,
	Singleplayer,
	Coop,
	Versus
};

class IEventHandler;

class SceneLoader final : public dae::Singleton<SceneLoader>
{
public:
	void LoadMainMenu();
	void LoadSinglePlayer();
	void LoadCoop();
	void LoadVersus();

	void LoadGameBackground();
	void ShowResultsScreen();

	void InitializeEventHandlers();

	std::vector<std::shared_ptr<IEventHandler>> GetEventhandlers() const { return m_EventHandlers; };

	GameMode GetCurrentGameMode() const { return m_CurrentGameMode; };
private:
	GameMode m_CurrentGameMode = GameMode::MainMenu;
	
	std::vector<std::shared_ptr<IEventHandler>> m_EventHandlers;

	void CleanUp()const;
};

