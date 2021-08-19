#include "pch.h"
#include "InputManager.h"

dae::InputManager::InputManager()
	:m_Buttons{ 
	ControllerButton::CB_A,
	ControllerButton::CB_B, 
	ControllerButton::CB_X, 
	ControllerButton::CB_Y,
	ControllerButton::CB_Up,
	ControllerButton::CB_Right, 
	ControllerButton::CB_Down, 
	ControllerButton::CB_Left , 
	ControllerButton::CB_Select,
	ControllerButton::CB_Start ,
	ControllerButton::CB_RightThumb,
	ControllerButton::CB_LeftThumb,
	ControllerButton::CB_RightShoulder,
	ControllerButton::CB_LeftShoulder }
	,m_CurrentState{}
{
	for (size_t i = 0; i < XUSER_MAX_COUNT; i++) m_Controllers.push_back(std::make_unique<Controller>());
}

void dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	XInputGetState(0, &m_CurrentState);
}

bool dae::InputManager::IsPressed(ControllerButton button, const  std::unique_ptr<Controller>& controller) const
{
	if (controller->m_CurrentState.Gamepad.wButtons & int(button)) return true;
	return false;
}

bool dae::InputManager::InputHandler()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&(m_Controllers[i]->m_CurrentState), sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &(m_Controllers[i]->m_CurrentState));

		for (ControllerButton button : m_Buttons)
		{

			if (m_Controllers[i]->m_ConsoleButtonCommands.find(button) == m_Controllers[i]->m_ConsoleButtonCommands.end())
				continue;

			const auto& command = m_Controllers[i]->m_ConsoleButtonCommands.at(button);

			if (!command->GetIsPressed())
			{
				if (IsPressed(button, m_Controllers[i]))
				{
					command->Execute();
					command->SetIsPressed(true);
				}
			}
			else
			{
				if (!IsPressed(button, m_Controllers[i]))
				{
					command->Release();
					command->SetIsPressed(false);
				}
			}
		}
	}
	return true;
}


bool dae::InputManager::KeyboardInput()
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || m_IsExiting)
			return false;

		if (event.type == SDL_KEYDOWN)
		{
			KeyboardButton key =KeyboardButton(int(event.key.keysym.sym));
			std::map<KeyboardButton, std::unique_ptr<Command>>& commandMap = m_KeyboardButtonCommands;

			if (commandMap.find(key) != commandMap.end())
			{
				const auto& command = commandMap.at(key);
				command->Execute();
				command->SetIsPressed(true);
			}
		}
		if (event.type == SDL_KEYUP)
		{
			KeyboardButton key = KeyboardButton(int(event.key.keysym.sym));
			std::map<KeyboardButton, std::unique_ptr<Command>>& commandMap = m_KeyboardButtonCommands;

			if (commandMap.find(key) != commandMap.end())
			{
				const auto& command = commandMap.at(key);
				command->Release();
				command->SetIsPressed(false);
			}
		}
	}
	return true;
}