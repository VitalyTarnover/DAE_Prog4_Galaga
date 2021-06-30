#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "SystemTime.h"


//components
#include "FPSTextComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

#include "Commands.h"
#include "AnalogStickCommand.h"
#include "AnalogTriggerCommand.h"

//will be moved to  loader
#include "LivesObserver.h"
#include "ScoreObserver.h"

#include "FighterShipMovementComponent.h"


using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());


	if (SDL_Init(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());

	//int frequency = 44100;
	//int chunkSize = 2048;
	//int channels = 2;

	//if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
	//	throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	m_WindowWidth = 1280;
	m_WindowHeight = 720;

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
	
}

void dae::Minigin::LoadGame() const
{
	srand(int(time(NULL)));

	dae::Scene& scene = SceneManager::GetInstance().CreateScene("Game");

	//fps counter
	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene.Add(go);

	//player

	

	auto playerFighter = std::make_shared<GameObject>("Player1");
	playerFighter->AddComponent(new TransformComponent(glm::vec3(m_WindowWidth/2, m_WindowHeight/2, 0)));
	playerFighter->AddComponent(new HealthComponent(3));
	playerFighter->AddComponent(new ScoreComponent(0));
	playerFighter->AddWatcher(new LivesObserver());
	playerFighter->AddWatcher(new ScoreObserver());
	playerFighter->AddComponent(new Texture2DComponent("FighterShip.png", scene.GetSceneScale()));
	playerFighter->AddComponent(new SpriteAnimComponent(2));
	playerFighter->AddComponent(new FighterShipMovementComponent(500));
	scene.Add(playerFighter);
	scene.AddPlayer(playerFighter);



}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	//AudioLocator::Reset();
	//Mix_Quit();
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



	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;


		input.ProcessInput();
		input.ControllerAnalogs();
		input.InputHandler();


		doContinue = input.KeyboardInput();

		SystemTime::GetInstance().SetDeltaTime(deltaTime);
	
		sceneManager.Update();
		renderer.Render();
	}

	Cleanup();
}

void dae::Minigin::BindCommands()
{
	auto& inputManager = InputManager::GetInstance();

	//keyboard
	inputManager.AssignKey<MoveLeft>(KeyboardButton::Left, 0);
	inputManager.AssignKey<MoveRight>(KeyboardButton::Right, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESC, 0);
}
