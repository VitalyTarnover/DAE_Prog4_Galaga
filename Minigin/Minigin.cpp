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


//Input
#include "Commands.h"
#include "AnalogStickCommand.h"
#include "AnalogTriggerCommand.h"


//Components
#include "FPSTextComponent.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

#include "FighterShipMovementComponent.h"
#include "BaseEnemyMovementComponent.h"
#include "EnemyFlyInMovement.h"
#include "PlayerHealthComponent.h"
#include "RenderComponent.h"

#include "TractorBeamDangerComponent.h"
#include "GalagaBackgroundComponent.h"

#include "ScoreComponent.h"

//Audio
#include "AudioLocator.h"
#include "AudioServiceProvider.h"

//ObserverV2
#include "ScoreEventHandler.h"
#include "HealthEventHandler.h"

//will be moved to loader
#include "Event.h"

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
	AudioLocator::GetAudioService().AddSoundToLibrary("someSound", "Resources/QbertDiscSFX.wav", false);
	
}

void dae::Minigin::LoadGame() const
{
	srand(int(time(NULL)));

	dae::Scene& scene = SceneManager::GetInstance().CreateScene("Game");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	//managers

	std::vector<std::shared_ptr<IEventHandler>> handlers;

	std::shared_ptr<HealthEventHandler> healthEventHandler = std::make_shared<HealthEventHandler>();

	handlers.push_back(healthEventHandler);

	CollisionManager::GetInstance().InitializeEvents(handlers);

	LevelManager::GetInstance().NextLevel();



	//Background
	auto background = std::make_shared<GameObject>("Background");
	background->AddComponent(new TransformComponent(glm::vec3(0,0,0)));
	background->AddComponent(new Texture2DComponent("StarsBackground.jpg"));
	background->AddComponent(new GalagaBackgroundComponent);
	background->AddComponent(new RenderComponent());
	scene.Add(background);

	auto background1 = std::make_shared<GameObject>("Background");
	background1->AddComponent(new TransformComponent(glm::vec3(0, -m_WindowHeight, 0)));
	background1->AddComponent(new Texture2DComponent("StarsBackground.jpg"));
	background1->AddComponent(new GalagaBackgroundComponent);
	background1->AddComponent(new RenderComponent());
	scene.Add(background1);

	//fps counter
	//auto go = std::make_shared<GameObject>("FPSCounter");
	//go->AddComponent(new TransformComponent(glm::vec3(0, m_WindowHeight - 20, 0)));
	//go->AddComponent(new FPSTextComponent(font2));
	//go->AddComponent(new RenderComponent());
	//scene.Add(go);




	//player
	auto playerFighter = std::make_shared<GameObject>("Player1");
	playerFighter->AddComponent(new TransformComponent(glm::vec3(m_WindowWidth / 2, m_WindowHeight / 5 * 4, 0), 15.f, 16.f, scene.GetSceneScale(), scene.GetSceneScale()));
	//playerFighter->AddComponent(new ScoreComponent(0));
	playerFighter->AddComponent(new PlayerHealthComponent(3));
	//playerFighter->AddWatcher(new ScoreObserver());
	playerFighter->AddComponent(new RenderComponent());
	playerFighter->AddComponent(new Texture2DComponent("FighterShip.png", scene.GetSceneScale()));//TODO: mby make a separate variable out of scale, we get it way too often
	playerFighter->AddComponent(new SpriteAnimComponent(2));
	playerFighter->AddComponent(new FighterShipMovementComponent(500));
	playerFighter->AddComponent(new ScoreComponent());
	scene.Add(playerFighter);
	scene.AddPlayer(playerFighter);
	CollisionManager::GetInstance().SetPlayersCollisions();//!!!

	//HUD
	auto player1Text = std::make_shared<GameObject>("Player1Text");
	player1Text->AddComponent(new TransformComponent(glm::vec3(10, 10, 0)));
	player1Text->AddComponent(new TextComponent("Player 1", font2, SDL_Color{ 255,255,255 }));
	player1Text->AddComponent(new RenderComponent());
	scene.Add(player1Text);

	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(10, 25, 0)));
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font2, SDL_Color{ 255,255,255 }));
	scoreDisplay->AddComponent(new RenderComponent());
	scene.Add(scoreDisplay);

	auto healthDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthDisplay->AddComponent(new TransformComponent(glm::vec3(10, 40, 0)));
	healthDisplay->AddComponent(new TextComponent(
		"Lives: " + std::to_string(playerFighter->GetComponent<PlayerHealthComponent>()->GetLives()), font2, SDL_Color{ 255,255,255 }));
	healthDisplay->AddComponent(new RenderComponent());
	scene.Add(healthDisplay);

	auto levelDisplay = std::make_shared<GameObject>("LevelDisplay");
	levelDisplay->AddComponent(new TransformComponent(glm::vec3(0, m_WindowHeight - 20, 0)));
	levelDisplay->AddComponent(new TextComponent("Level: "+std::to_string(LevelManager::GetInstance().GetCurrentLevel()), font2, SDL_Color{ 255,255,255 }));
	levelDisplay->AddComponent(new RenderComponent());
	scene.Add(levelDisplay);


	auto textTest = std::make_shared<GameObject>("Text");
	textTest->AddComponent(new TransformComponent(glm::vec3(200, 200, 0)) );
	textTest->AddComponent(new TextComponent("textTestText", font2, SDL_Color{ 255,0,255 }));
	textTest->AddComponent(new RenderComponent());
	scene.Add(textTest);
	

	//TractorBeam test
	
	//glm::vec3 tractorBeam1Pos{ m_WindowWidth / 2, m_WindowHeight / 2, 0 };
	//glm::vec3 tractorBeam2Pos{ tractorBeam1Pos.x, tractorBeam1Pos.y + (18 * scene.GetSceneScale()), 0 };
	//glm::vec3 tractorBeam3Pos{ tractorBeam2Pos.x, tractorBeam2Pos.y + (16 * scene.GetSceneScale()), 0 };
	//
	//auto tractorBeam1 = std::make_shared<GameObject>("tractorBeam1");
	//tractorBeam1->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	//tractorBeam1->AddComponent(new Texture2DComponent("TractorBeam1.png", scene.GetSceneScale()));
	////tractorBeam1->AddComponent(new SpriteAnimComponent(3));
	////tractorBeam1->AddComponent(new TractorBeamComponent(10));
	//tractorBeam1->AddComponent(new RenderComponent());
	//scene.Add(tractorBeam1);
	

	//auto tractorBeam2 = std::make_shared<GameObject>("tractorBeam2");
	//tractorBeam2->AddComponent(new TransformComponent(tractorBeam2Pos));
	//tractorBeam2->AddComponent(new Texture2DComponent("TractorBeam2.png", scene.GetSceneScale()));
	//tractorBeam2->AddComponent(new SpriteAnimComponent(3));
	//tractorBeam2->AddComponent(new TractorBeamComponent(7));
	//tractorBeam2->AddComponent(new RenderComponent());
	//scene.Add(tractorBeam2);
	//
	//
	//auto tractorBeam3 = std::make_shared<GameObject>("tractorBeam3");
	//tractorBeam3->AddComponent(new TransformComponent(tractorBeam3Pos));
	//tractorBeam3->AddComponent(new Texture2DComponent("TractorBeam3.png", scene.GetSceneScale()));
	//tractorBeam3->AddComponent(new SpriteAnimComponent(3));
	//tractorBeam3->AddComponent(new TractorBeamDangerComponent(4));
	//tractorBeam3->AddComponent(new RenderComponent());
	//scene.Add(tractorBeam3);




	//enemies

	//for (size_t i = 0; i < 10; i++)
	//{
	//	auto testEnemyShip = std::make_shared<GameObject>("TestEnemy");
	//	testEnemyShip->AddComponent(new TransformComponent(glm::vec3(m_WindowWidth / 12 * (i+1), m_WindowHeight / 5 * 2, 0), 13.f, 10.f, scene.GetSceneScale(), scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new Texture2DComponent("Bee.png", scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new SpriteAnimComponent(2));
	//	testEnemyShip->AddComponent(new BaseEnemyMovementComponent(0.75f, 5, 5));//steps amount should be odd, so it is properly and cheaply animated
	//	scene.Add(testEnemyShip);
	//	CollisionManager::GetInstance().AddGameObjectForCheck(true, testEnemyShip);
	//}
	//
	//for (size_t i = 0; i < 10; i++)
	//{
	//	auto testEnemyShip = std::make_shared<GameObject>("TestEnemy");
	//	testEnemyShip->AddComponent(new TransformComponent(glm::vec3(m_WindowWidth / 12 * (i + 1.5f), m_WindowHeight / 5 , 0), 13.f, 10.f, scene.GetSceneScale(), scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new Texture2DComponent("Bee.png", scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new SpriteAnimComponent(2));
	//	testEnemyShip->AddComponent(new BaseEnemyMovementComponent(0.75f, 5, 5));//steps amount should be odd, so it is properly and cheaply animated
	//	scene.Add(testEnemyShip);
	//	CollisionManager::GetInstance().AddGameObjectForCheck(true, testEnemyShip);
	//}


	//{
	//	auto testEnemyShip = std::make_shared<GameObject>("TestEnemy");
	//	testEnemyShip->AddComponent(new TransformComponent(glm::vec3(0, 0, 0), 13.f, 10.f, scene.GetSceneScale(), scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new Texture2DComponent("Bee.png", scene.GetSceneScale()));
	//	testEnemyShip->AddComponent(new SpriteAnimComponent(2));
	//	testEnemyShip->AddComponent(new EnemyFlyInMovement());
	//	scene.Add(testEnemyShip);
	//	CollisionManager::GetInstance().AddGameObjectForCheck(true, testEnemyShip);
	//}




	//std::vector<glm::vec2> enemyPositions{};
	//
	//for (size_t i = 0; i < 3; i++)
	//{
	//	enemyPositions.push_back(glm::vec2{ m_WindowWidth / 11 * (i + 4), m_WindowHeight / 7 });
	//}
	//
	//for (size_t i = 0; i < 10; i++)
	//{
	//	enemyPositions.push_back(glm::vec2{ m_WindowWidth / 12 * (i + 1), m_WindowHeight / 7 * 3 });
	//}
	//
	//for (size_t i = 0; i < 10; i++)
	//{
	//	enemyPositions.push_back(glm::vec2{ m_WindowWidth / 12 * (i + 1.5f), m_WindowHeight / 7 * 2 });
	//}



	//std::vector<std::vector<int>> beeInfo{};
	//std::vector<std::vector<int>> bFInfo{};
	//std::vector<std::vector<int>> birdInfo{};
	//
	//int birdsXPosDivisor = 9;
	//for (int i = 0; i < 3; i++)//Birds
	//{
	//	std::vector<int> newbirdInfo{};
	//	newbirdInfo.push_back(i + 3);//4 5 6
	//	newbirdInfo.push_back(birdsXPosDivisor);
	//	newbirdInfo.push_back(i + 1);
	//
	//	birdInfo.push_back(newbirdInfo);
	//}
	//
	//int bfXPosDivisor = 12;
	//for (int i = 0; i < 10; i++)//BFs
	//{
	//	std::vector<int> newBFInfo{};
	//	newBFInfo.push_back(i + 1);
	//	newBFInfo.push_back(bfXPosDivisor);
	//	newBFInfo.push_back(i % 2);
	//	if (i == 2)newBFInfo.push_back(1);//4 as well
	//	else if (i == 4)newBFInfo.push_back(2);//6 as well
	//	else if (i == 6)newBFInfo.push_back(3);//8 as well
	//	else newBFInfo.push_back(-1);
	//	
	//	bFInfo.push_back(newBFInfo);
	//}
	//
	//int beeXPosDivisor = 12;
	//for (int i = 0; i < 10; i++)//Bees
	//{
	//	std::vector<int> newBeeInfo{};
	//	newBeeInfo.push_back(i+1);
	//	newBeeInfo.push_back(beeXPosDivisor);
	//	newBeeInfo.push_back(i%2);
	//
	//	beeInfo.push_back(newBeeInfo);
	//}

	//enemyPositions.push_back(glm::vec2{ m_WindowWidth / 12 * (4), m_WindowHeight / 5 * 2 });



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

	//AudioLocator::GetAudioService().QueueSound("someSound", 30.f);

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

	//keyboard
	inputManager.AssignKey<ShootRocket>(KeyboardButton::A, 0);
	inputManager.AssignKey<MoveLeft>(KeyboardButton::Left, 0);
	inputManager.AssignKey<MoveRight>(KeyboardButton::Right, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESC, 0);
}

void dae::Minigin::UpdateManagers()
{
	CollisionManager::GetInstance().Update();
	EnemyManager::GetInstance().Update();
	ExplosionManager::GetInstance().Update();
}
