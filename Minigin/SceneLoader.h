#pragma once
#include "Singleton.h"
#include "Event.h"

enum class GameMode
{
	MainMenu,
	Singleplayer,
	Coop,
	Versus
};

class SceneLoader final : public dae::Singleton<SceneLoader>
{
public:
	void LoadMainMenu();
	void LoadSinglePlayer();
	void LoadCoop();
	void LoadVersus();

	void LoadGameBackground() const;
	void ShowResultsScreen() const;

	void InitializeEventHandlers();

	std::vector<std::shared_ptr<IEventHandler>> GetEventhandlers() const { return m_EventHandlers; };

	GameMode GetCurrentGameMode() const { return m_CurrentGameMode; };
private:
	Event m_EventGameEnd;
	GameMode m_CurrentGameMode = GameMode::MainMenu;
	
	std::vector<std::shared_ptr<IEventHandler>> m_EventHandlers;

	void CleanUp()const;
};

