#pragma once
#include <iostream>

#include "BaseCommand.h"

#include "SceneManager.h"
#include "Scene.h"

#include "FighterShipMovementComponent.h"
#include "RocketManager.h"
#include "BirdMovementComponent.h"

#include "SceneLoader.h"


class LoadSinglePlayerCommand : public Command
{
public:
	LoadSinglePlayerCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadSinglePlayerCommand() override = default;
	void Execute() const override {
		if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::MainMenu)
			SceneLoader::GetInstance().LoadSinglePlayer();
	};
	void Release() const override {};
};


class LoadCoopCommand : public Command
{
public:
	LoadCoopCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadCoopCommand() override = default;
	void Execute() const override {
		if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::MainMenu)
			SceneLoader::GetInstance().LoadCoop();
	};
	void Release() const override {};
};


class LoadVersusCommand : public Command
{
public:
	LoadVersusCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadVersusCommand() override = default;
	void Execute() const override {
		if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::MainMenu)
			SceneLoader::GetInstance().LoadVersus();
	};
	void Release() const override {};
};


class LoadMainMenuCommand : public Command
{
public:
	LoadMainMenuCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadMainMenuCommand() override = default;
	void Execute() const override {
		if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::MainMenu)
			SceneLoader::GetInstance().LoadMainMenu();
	};
	void Release() const override {};
};




class ExitCommand final : public Command
{
public:
	ExitCommand(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override { dae::InputManager::GetInstance().SetIsExiting(true); }
	void Release() const override {};
};

class ShootRocket : public Command
{
public:
	ShootRocket(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~ShootRocket() override = default;
	void Execute() const override {
		//if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::MainMenu)
		//RocketManager::GetInstance().SpawnPlayerRocket(m_ControllerIndex);

		GameMode currentGameMode = SceneLoader::GetInstance().GetCurrentGameMode();

		std::shared_ptr<GameObject> pPlayerActor = nullptr;


		switch (currentGameMode)
		{
		case GameMode::MainMenu:
			break;
		case GameMode::Singleplayer:
			RocketManager::GetInstance().SpawnPlayerRocket(0);
			break;
		case GameMode::Coop:
			RocketManager::GetInstance().SpawnPlayerRocket(m_ControllerIndex);
			break;
		case GameMode::Versus:
			if (m_ControllerIndex == 0) RocketManager::GetInstance().SpawnPlayerRocket(0);
			else RocketManager::GetInstance().SpawnEnemyRocket();
			break;
		default:
			break;
		}
	};
	void Release() const override {};
};


class MoveLeft final : public Command
{
public:
	MoveLeft(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		GameMode currentGameMode = SceneLoader::GetInstance().GetCurrentGameMode();

		std::shared_ptr<GameObject> pPlayerActor = nullptr;


		switch (currentGameMode)
		{
		case GameMode::MainMenu:
			break;
		case GameMode::Singleplayer:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(true);
			break;
		case GameMode::Coop:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(true);
			break;
		case GameMode::Versus:
			if (m_ControllerIndex == 0)
			{
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(true);
			}
			else
			{
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
				if (pPlayerActor)pPlayerActor->GetComponent<BirdMovementComponent>()->Switch();
			}
			break;
		default:
			break;
		}

	}

	void Release() const override
	{
		if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::MainMenu)
		{
			GameMode currentGameMode = SceneLoader::GetInstance().GetCurrentGameMode();

			std::shared_ptr<GameObject> pPlayerActor = nullptr;


			switch (currentGameMode)
			{
			case GameMode::MainMenu:
				break;
			case GameMode::Singleplayer:
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(true);
				break;
			case GameMode::Coop:
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
				pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(true);
				break;
			case GameMode::Versus:
				if (m_ControllerIndex == 0)
				{
					pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
					pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(true);
				}
				break;
			default:
				break;
			}
		}
	};
};

class MoveRight final : public Command
{
public:
	MoveRight(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		GameMode currentGameMode = SceneLoader::GetInstance().GetCurrentGameMode();

		std::shared_ptr<GameObject> pPlayerActor = nullptr;

		switch (currentGameMode)
		{
		case GameMode::MainMenu:
			break;
		case GameMode::Singleplayer:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(false);
			break;
		case GameMode::Coop:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(false);
			break;
		case GameMode::Versus:
			if (m_ControllerIndex == 0)
			{
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(false);
			}
			break;
		default:
			break;
		}
	}

	void Release() const override
	{
		GameMode currentGameMode = SceneLoader::GetInstance().GetCurrentGameMode();

		std::shared_ptr<GameObject> pPlayerActor = nullptr;

		switch (currentGameMode)
		{
		case GameMode::MainMenu:
			break;
		case GameMode::Singleplayer:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(false);
			break;
		case GameMode::Coop:
			pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
			pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(false);
			break;
		case GameMode::Versus:
			if (m_ControllerIndex == 0)
			{
				pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(false);
			}
			break;
		default:
			break;
		}
	};
};



