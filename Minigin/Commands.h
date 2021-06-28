#pragma once
#include <iostream>

#include "Components.h"
#include "BaseCommand.h"

#include "SceneManager.h"
#include "Scene.h"

#include "FighterShipMovementComponent.h"



class LoadSinglePlayerCommand : public Command
{
public:
	LoadSinglePlayerCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadSinglePlayerCommand() override = default;
	void Execute() const override {};
	void Release() const override {};
	void Undo() override {};
};


class LoadCoopCommand : public Command
{
public:
	LoadCoopCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadCoopCommand() override = default;
	void Execute() const override {};
	void Release() const override {};
	void Undo() override {};
};


class LoadVersusCommand : public Command
{
public:
	LoadVersusCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadVersusCommand() override = default;
	void Execute() const override {};
	void Release() const override {};
	void Undo() override {};
};


class LoadMainMenuCommand : public Command
{
public:
	LoadMainMenuCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadMainMenuCommand() override = default;
	void Execute() const override {};
	void Release() const override {};
	void Undo() override {};
};




class ExitCommand final : public Command
{
public:
	ExitCommand(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override { dae::InputManager::GetInstance().SetIsExiting(true); }
	void Release() const override {};
	void Undo() override {};
};



class MoveLeft final : public Command
{
public:
	MoveLeft(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(true);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(true);
	};
	void Undo() override {};
};

class MoveRight final : public Command
{
public:
	MoveRight(int index) : Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<FighterShipMovementComponent>()->StartMoving(false);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<FighterShipMovementComponent>()->StopMoving(false);
	};
	void Undo() override {};
};



