#pragma once
#include "Singleton.h"

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


	GameMode GetCurrentGameMode() const { return m_CurrentGameMode; };
private:
	GameMode m_CurrentGameMode;
	
	void CleanUp()const;
};

