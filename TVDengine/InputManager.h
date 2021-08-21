#pragma once

#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include "BaseCommand.h"
#include <vector>
#include <map>
#include "Singleton.h"
#include <SDL.h>
#pragma comment(lib, "XInput.lib")

namespace dae
{
	enum class KeyboardButton
	{
		A = SDLK_a,//P1 Left
		D = SDLK_d,//P1 Right
		F = SDLK_f,//P1 Shoot
		J = SDLK_j,//P2 Left
		L = SDLK_l,//P2 Right
		H = SDLK_h,//P2 Shoot
		P = SDLK_p,//pause/main menu
		Enter = SDLK_RETURN,
		One = SDLK_1,
		Two = SDLK_2,
		Three = SDLK_3,
		ESC = SDLK_ESCAPE
	};

	enum class ControllerButton
	{
		CB_A = XINPUT_GAMEPAD_A,
		CB_B = XINPUT_GAMEPAD_B,
		CB_X = XINPUT_GAMEPAD_X,
		CB_Y = XINPUT_GAMEPAD_Y,
		CB_Select = XINPUT_GAMEPAD_BACK,
		CB_Up = XINPUT_GAMEPAD_DPAD_UP,
		CB_Right = XINPUT_GAMEPAD_DPAD_RIGHT,
		CB_Down = XINPUT_GAMEPAD_DPAD_DOWN,
		CB_Left = XINPUT_GAMEPAD_DPAD_LEFT,
		CB_LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		CB_RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		CB_LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		CB_RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		CB_Start = XINPUT_GAMEPAD_START,
	};

	struct Controller 
	{
		std::map<ControllerButton, std::unique_ptr<Command>> m_ConsoleButtonCommands;
		XINPUT_STATE m_CurrentState{};
	};

	struct Keyboard
	{
		std::map<KeyboardButton, std::unique_ptr<Command>> m_ConsoleCommands;
	};


	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		void ProcessInput();
		
		bool KeyboardInput();
		
		bool InputHandler();
		
		template <typename T>
		void AssignKey(KeyboardButton button, int playerIndex = 0)
		{
			m_KeyboardButtonCommands.insert(std::make_pair(button,std::make_unique<T>(playerIndex)));
		}

		template <typename T>
		void AssignKey(ControllerButton button, int controllerIndex, int playerIndex)
		{
			m_Controllers[controllerIndex]->m_ConsoleButtonCommands.insert(std::make_pair(button, std::make_unique<T>(playerIndex)));
		}

		void SetIsExiting(bool exit) { m_IsExiting = exit; }

	private:
		bool IsPressed(ControllerButton button, const std::unique_ptr<Controller>& controller) const;

		XINPUT_STATE m_CurrentState;
		
		bool m_IsExiting = false;

		//buttons
		static const int amountOfButtons = 14;
		const ControllerButton m_Buttons[amountOfButtons];
		
		//controllers
		std::vector<std::unique_ptr<Controller>> m_Controllers;

		//keyboard
		std::map<KeyboardButton, std::unique_ptr<Command>> m_KeyboardButtonCommands;
	};
}
