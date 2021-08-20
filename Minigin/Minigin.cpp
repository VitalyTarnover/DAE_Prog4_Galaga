#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "SystemTime.h"

//Managers
#include "InputManager.h"
#include "CollisionManager.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "ExplosionManager.h"
#include "SceneLoader.h"

//Input
#include "Commands.h"
#include "AnalogStickCommand.h"
#include "AnalogTriggerCommand.h"

//ObserverV2
#include "AudioEventHandler.h"


//Audio
#include "AudioLocator.h"
#include "AudioServiceProvider.h"



//will be moved to loader
//#include "Event.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());


	if (SDL_Init(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	m_WindowWidth = 1280;
	m_WindowHeight = 720;
	dae::SceneManager::GetInstance().SetScreenWidthHeight(m_WindowWidth, m_WindowHeight);

	m_Window = SDL_CreateWindow(
		"DAE_Prog4_Galaga (by Tarnover Vitaly)",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowWidth,
		m_WindowHeight,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);


	AudioLocator::SetAudioService(new AudioServiceProvider());
	AudioLocator::GetAudioService().AddSoundToLibrary("enemyDeath", "Resources/SFX/enemyDeathSFX.wav", true);
	AudioLocator::GetAudioService().AddSoundToLibrary("enemyShot", "Resources/SFX/enemyShotSFX.wav", true);
	AudioLocator::GetAudioService().AddSoundToLibrary("playerShot", "Resources/SFX/playerShotSFX.wav", true);
	AudioLocator::GetAudioService().AddSoundToLibrary("gameEnd", "Resources/SFX/gameEndSFX.wav", true);
	AudioLocator::GetAudioService().AddSoundToLibrary("playerDeath", "Resources/SFX/playerDeathSFX.wav", true);
	
}

void dae::Minigin::LoadGame() const
{
	srand(int(time(NULL)));
	
	SceneManager::GetInstance().CreateScene("Game");
	
	SceneLoader::GetInstance().InitializeEventHandlers();

	SceneLoader::GetInstance().LoadMainMenu();
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	AudioLocator::Reset();
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };

	BindCommands();

	std::thread audioThread(&AudioService::Update, &AudioLocator::GetAudioService());

	//AudioLocator::GetAudioService().QueueSound("gameEnd", 30.f);

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;


		input.ProcessInput();
		input.InputHandler();


		doContinue = input.KeyboardInput();

		SystemTime::GetInstance().SetDeltaTime(deltaTime);
	
		UpdateManagers();
		sceneManager.Update();
		renderer.Render();

		std::this_thread::sleep_for(std::chrono::milliseconds(int(duration<float>(currentTime - high_resolution_clock::now()).count()) + m_MsPerFrame ));
	}

	//join - blocks until the thread of execution that's associated with the calling object completes.
	//detach - detaches the associated thread. The operating system becomes responsible for releasing thread resources on termination.
	audioThread.detach();
	Cleanup();
}

void dae::Minigin::BindCommands()
{
	auto& inputManager = InputManager::GetInstance();

	//keyboard => Player 1
	inputManager.AssignKey<ShootRocket>(KeyboardButton::F, 0);
	inputManager.AssignKey<MoveLeft>(KeyboardButton::A, 0);
	inputManager.AssignKey<MoveRight>(KeyboardButton::D, 0);

	//keyboard => Player 2
	inputManager.AssignKey<ShootRocket>(KeyboardButton::H, 1);
	inputManager.AssignKey<MoveLeft>(KeyboardButton::J, 1);
	inputManager.AssignKey<MoveRight>(KeyboardButton::L, 1);

	//keyboard => Menu + System
	inputManager.AssignKey<LoadSinglePlayerCommand>(KeyboardButton::One, 0);
	inputManager.AssignKey<LoadCoopCommand>(KeyboardButton::Two, 0);
	inputManager.AssignKey<LoadVersusCommand>(KeyboardButton::Three, 0);
	inputManager.AssignKey<LoadMainMenuCommand>(KeyboardButton::P, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESC, 0);

	//first gamepad => Player 2
	inputManager.AssignKey<ShootRocket>(ControllerButton::CB_A,0,1);
	inputManager.AssignKey<MoveLeft>(ControllerButton::CB_Left,0,1);
	inputManager.AssignKey<MoveRight>(ControllerButton::CB_Right,0,1);

}

void dae::Minigin::UpdateManagers()
{
	if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::MainMenu)
	{
		CollisionManager::GetInstance().Update();
		EnemyManager::GetInstance().Update();
		ExplosionManager::GetInstance().Update();
		LevelManager::GetInstance().Update();
	}
}
