#include "MiniginPCH.h"
#include "SceneLoader.h"

#include "GameObject.h"
#include "Scene.h"

//ObserverV2
#include "ScoreEventHandler.h"
#include "HealthEventHandler.h"

//Managers
#include "CollisionManager.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "SceneLoader.h"
#include "RocketManager.h"

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

using namespace dae;

void SceneLoader::LoadMainMenu()
{
	m_CurrentGameMode = GameMode::MainMenu;

	CleanUp();

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	auto scene = SceneManager::GetInstance().GetCurrentScene();

	int screenWidth = SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = SceneManager::GetInstance().GetScreenHeight();

	//Logo
	float logoWidth = 600.f;
	float logoHeight = 310.f;
	auto galagaLogo = std::make_shared<GameObject>("Logo");
	galagaLogo->AddComponent(new TransformComponent(glm::vec3(screenWidth/2 - (logoWidth / 2),0,0), logoWidth, logoHeight,1.f,1.f));
	galagaLogo->AddComponent(new Texture2DComponent("Galaga_logo.png"));
	galagaLogo->AddComponent(new RenderComponent());
	scene->Add(galagaLogo);

	//Texts
	int verticalOffset = 0;
	
	auto text1 = std::make_shared<GameObject>("Controls");
	text1->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text1->AddComponent(new TextComponent("Controls:", font, SDL_Color{ 255,0,0 }));
	text1->AddComponent(new RenderComponent());
	scene->Add(text1);

	verticalOffset += 15;

	auto text2 = std::make_shared<GameObject>("Controls1");
	text2->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text2->AddComponent(new TextComponent("Player 1: Left/Right arrow - move left/right, A - shoot.", font, SDL_Color{ 255,255,255 }));
	text2->AddComponent(new RenderComponent());
	scene->Add(text2);
	
	verticalOffset += 15;

	auto text3 = std::make_shared<GameObject>("Controls2");
	text3->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text3->AddComponent(new TextComponent("Player 2: J/L - move left/right, H - shoot.", font, SDL_Color{ 255,255,255 }));
	text3->AddComponent(new RenderComponent());
	scene->Add(text3);

	verticalOffset += 30;

	auto text4 = std::make_shared<GameObject>("GameModes");
	text4->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text4->AddComponent(new TextComponent("Game modes:", font, SDL_Color{ 255,0,0 }));
	text4->AddComponent(new RenderComponent());
	scene->Add(text4);

	verticalOffset += 15;

	auto text5 = std::make_shared<GameObject>("Singleplayer");
	text5->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text5->AddComponent(new TextComponent("Press 1 to load singleplayer mode", font, SDL_Color{ 255,255,255 }));
	text5->AddComponent(new RenderComponent());
	scene->Add(text5);


	verticalOffset += 15;

	auto text6 = std::make_shared<GameObject>("Coop");
	text6->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text6->AddComponent(new TextComponent("Press 2 to load coop mode", font, SDL_Color{ 255,255,255 }));
	text6->AddComponent(new RenderComponent());
	scene->Add(text6);


	verticalOffset += 15;

	auto text7 = std::make_shared<GameObject>("Versus");
	text7->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text7->AddComponent(new TextComponent("Press 3 to load versus mode", font, SDL_Color{ 255,255,255 }));
	text7->AddComponent(new RenderComponent());
	scene->Add(text7);

	verticalOffset += 15;

	auto text8 = std::make_shared<GameObject>("MainMenu");
	text8->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text8->AddComponent(new TextComponent("Press P in any game mode to return to the main menu", font, SDL_Color{ 255,255,255 }));
	text8->AddComponent(new RenderComponent());
	scene->Add(text8);

	verticalOffset += 30;

	auto text9 = std::make_shared<GameObject>("Escape");
	text9->AddComponent(new TransformComponent(glm::vec3(0, screenHeight / 2 + verticalOffset, 0)));
	text9->AddComponent(new TextComponent("Press Esc in main menu to close the game", font, SDL_Color{ 255,255,255 }));
	text9->AddComponent(new RenderComponent());
	scene->Add(text9);

}

void SceneLoader::LoadSinglePlayer()
{
	m_CurrentGameMode = GameMode::Singleplayer;

	CleanUp();

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	auto scene = SceneManager::GetInstance().GetCurrentScene();
	
	int screenWidth = SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = SceneManager::GetInstance().GetScreenHeight();
	
	//managers
	std::vector<std::shared_ptr<IEventHandler>> handlers;

	std::shared_ptr<HealthEventHandler> healthEventHandler = std::make_shared<HealthEventHandler>();

	handlers.push_back(healthEventHandler);

	CollisionManager::GetInstance().InitializeEvents(handlers);

	LevelManager::GetInstance().NextLevel();

	LoadGameBackground();

	//fps counter //TODO: delete
	//auto go = std::make_shared<GameObject>("FPSCounter");
	//go->AddComponent(new TransformComponent(glm::vec3(0, m_WindowHeight - 20, 0)));
	//go->AddComponent(new FPSTextComponent(font2));
	//go->AddComponent(new RenderComponent());
	//scene->Add(go);


	//player
	auto playerFighter = std::make_shared<GameObject>("Player1");
	playerFighter->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2, screenHeight / 5 * 4, 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
	//playerFighter->AddComponent(new ScoreComponent(0));
	playerFighter->AddComponent(new PlayerHealthComponent(3));
	//playerFighter->AddWatcher(new ScoreObserver());
	playerFighter->AddComponent(new RenderComponent());
	playerFighter->AddComponent(new Texture2DComponent("FighterShip.png", scene->GetSceneScale()));//TODO: mby make a separate variable out of scale, we get it way too often
	playerFighter->AddComponent(new SpriteAnimComponent(2));
	playerFighter->AddComponent(new FighterShipMovementComponent(500));
	playerFighter->AddComponent(new ScoreComponent());
	scene->Add(playerFighter);
	scene->AddPlayer(playerFighter);
	CollisionManager::GetInstance().AddPlayerCollision(playerFighter);//!!!

	//HUD
	auto player1Text = std::make_shared<GameObject>("Player1Text");
	player1Text->AddComponent(new TransformComponent(glm::vec3(10, 10, 0)));
	player1Text->AddComponent(new TextComponent("Player 1", font, SDL_Color{ 255,255,255 }));
	player1Text->AddComponent(new RenderComponent());
	scene->Add(player1Text);

	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(10, 25, 0)));
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 }));
	scoreDisplay->AddComponent(new RenderComponent());
	scene->Add(scoreDisplay);

	auto healthDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthDisplay->AddComponent(new TransformComponent(glm::vec3(10, 40, 0)));
	healthDisplay->AddComponent(new TextComponent(
		"Lives: " + std::to_string(playerFighter->GetComponent<PlayerHealthComponent>()->GetLives()), font, SDL_Color{ 255,255,255 }));
	healthDisplay->AddComponent(new RenderComponent());
	scene->Add(healthDisplay);

	auto levelDisplay = std::make_shared<GameObject>("LevelDisplay");
	levelDisplay->AddComponent(new TransformComponent(glm::vec3(0, screenHeight - 20, 0)));
	levelDisplay->AddComponent(new TextComponent("Level: " + std::to_string(LevelManager::GetInstance().GetCurrentLevel()), font, SDL_Color{ 255,255,255 }));
	levelDisplay->AddComponent(new RenderComponent());
	scene->Add(levelDisplay);

}

void SceneLoader::LoadCoop()
{
	m_CurrentGameMode = GameMode::Coop;

	CleanUp();

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	auto scene = SceneManager::GetInstance().GetCurrentScene();

	int screenWidth = SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = SceneManager::GetInstance().GetScreenHeight();

	//managers
	std::vector<std::shared_ptr<IEventHandler>> handlers;

	std::shared_ptr<HealthEventHandler> healthEventHandler = std::make_shared<HealthEventHandler>();

	handlers.push_back(healthEventHandler);

	CollisionManager::GetInstance().InitializeEvents(handlers);

	LevelManager::GetInstance().NextLevel();

	LoadGameBackground();


	//player1
	auto playerFighter = std::make_shared<GameObject>("Player1");
	playerFighter->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2 - 20, screenHeight / 5 * 4, 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
	//playerFighter->AddComponent(new ScoreComponent(0));
	playerFighter->AddComponent(new PlayerHealthComponent(3));
	//playerFighter->AddWatcher(new ScoreObserver());
	playerFighter->AddComponent(new RenderComponent());
	playerFighter->AddComponent(new Texture2DComponent("FighterShip.png", scene->GetSceneScale()));//TODO: mby make a separate variable out of scale, we get it way too often
	playerFighter->AddComponent(new SpriteAnimComponent(2));
	playerFighter->AddComponent(new FighterShipMovementComponent(500));
	playerFighter->AddComponent(new ScoreComponent());
	scene->Add(playerFighter);
	scene->AddPlayer(playerFighter);
	CollisionManager::GetInstance().AddPlayerCollision(playerFighter);

	//HUD
	auto player1Text = std::make_shared<GameObject>("Player1Text");
	player1Text->AddComponent(new TransformComponent(glm::vec3(10, 10, 0)));
	player1Text->AddComponent(new TextComponent("Player 1", font, SDL_Color{ 255,255,255 }));
	player1Text->AddComponent(new RenderComponent());
	scene->Add(player1Text);

	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(10, 25, 0)));
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 }));
	scoreDisplay->AddComponent(new RenderComponent());
	scene->Add(scoreDisplay);

	auto healthDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthDisplay->AddComponent(new TransformComponent(glm::vec3(10, 40, 0)));
	healthDisplay->AddComponent(new TextComponent(
		"Lives: " + std::to_string(playerFighter->GetComponent<PlayerHealthComponent>()->GetLives()), font, SDL_Color{ 255,255,255 }));
	healthDisplay->AddComponent(new RenderComponent());
	scene->Add(healthDisplay);

	auto levelDisplay = std::make_shared<GameObject>("LevelDisplay");
	levelDisplay->AddComponent(new TransformComponent(glm::vec3(0, screenHeight - 20, 0)));
	levelDisplay->AddComponent(new TextComponent("Level: " + std::to_string(LevelManager::GetInstance().GetCurrentLevel()), font, SDL_Color{ 255,255,255 }));
	levelDisplay->AddComponent(new RenderComponent());
	scene->Add(levelDisplay);


	//player2
	auto playerFighter2 = std::make_shared<GameObject>("Player2");
	playerFighter2->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2 + 20, screenHeight / 5 * 4, 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
	playerFighter2->AddComponent(new PlayerHealthComponent(3));
	playerFighter2->AddComponent(new RenderComponent());
	playerFighter2->AddComponent(new Texture2DComponent("FighterShip2.png", scene->GetSceneScale()));//TODO: mby make a separate variable out of scale, we get it way too often
	playerFighter2->AddComponent(new FighterShipMovementComponent(500));
	playerFighter2->AddComponent(new ScoreComponent());
	scene->Add(playerFighter2);
	scene->AddPlayer(playerFighter2);

	//HUD2
	int horizontalOffset = 110;
	auto player2Text = std::make_shared<GameObject>("Player2Text");
	player2Text->AddComponent(new TransformComponent(glm::vec3(screenWidth - horizontalOffset, 10, 0)));
	player2Text->AddComponent(new TextComponent("Player 2", font, SDL_Color{ 190,190,255 }));
	player2Text->AddComponent(new RenderComponent());
	scene->Add(player2Text);

	auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
	scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(screenWidth - horizontalOffset, 25, 0)));
	scoreDisplay2->AddComponent(new TextComponent("Score: 0", font, SDL_Color{ 190,190,255 }));
	scoreDisplay2->AddComponent(new RenderComponent());
	scene->Add(scoreDisplay2);

	auto healthDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
	healthDisplay2->AddComponent(new TransformComponent(glm::vec3(screenWidth - horizontalOffset, 40, 0)));
	healthDisplay2->AddComponent(new TextComponent(
		"Lives: " + std::to_string(playerFighter->GetComponent<PlayerHealthComponent>()->GetLives()), font, SDL_Color{ 190,190,255 }));
	healthDisplay2->AddComponent(new RenderComponent());
	scene->Add(healthDisplay2);

	CollisionManager::GetInstance().AddPlayerCollision(playerFighter2);//!!!
}

void SceneLoader::LoadVersus()
{

	LoadSinglePlayer();

	m_CurrentGameMode = GameMode::Versus;
	//
	//CleanUp();
	//
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	//
	//auto scene = SceneManager::GetInstance().GetCurrentScene();
	//
	//int screenWidth = SceneManager::GetInstance().GetScreenWidth();
	//int screenHeight = SceneManager::GetInstance().GetScreenHeight();
	//
	////managers
	//std::vector<std::shared_ptr<IEventHandler>> handlers;
	//
	//std::shared_ptr<HealthEventHandler> healthEventHandler = std::make_shared<HealthEventHandler>();
	//
	//handlers.push_back(healthEventHandler);
	//
	//CollisionManager::GetInstance().InitializeEvents(handlers);
	//
	//LevelManager::GetInstance().NextLevel();
	//
	//LoadGameBackground();

}

void SceneLoader::LoadGameBackground()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	int screenHeight = SceneManager::GetInstance().GetScreenHeight();

	//Background
	auto background = std::make_shared<GameObject>("Background");
	background->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	background->AddComponent(new Texture2DComponent("StarsBackground.jpg"));
	background->AddComponent(new GalagaBackgroundComponent);
	background->AddComponent(new RenderComponent());
	scene->Add(background);

	auto background1 = std::make_shared<GameObject>("Background");
	background1->AddComponent(new TransformComponent(glm::vec3(0, -screenHeight, 0)));
	background1->AddComponent(new Texture2DComponent("StarsBackground.jpg"));
	background1->AddComponent(new GalagaBackgroundComponent);
	background1->AddComponent(new RenderComponent());
	scene->Add(background1);

}

void SceneLoader::ShowResultsScreen()
{
	int shots = RocketManager::GetInstance().GetNumberOfShotsFired();
	int hits = RocketManager::GetInstance().GetNumberOfHits();
	int ratio = 0;//TODO: mby use round here
	
	if (shots != 0)
	{
		ratio = int((float(hits) / shots) * 100);
	}

	int screenWidth = SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = SceneManager::GetInstance().GetScreenHeight();
	
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	int fontSize = 36;
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize);
	
	
	int verticalOffset = 0;

	std::string resultsText = "-RESULTS-";
	auto results = std::make_shared<GameObject>("results");
	results->AddComponent(new TransformComponent(glm::vec3(screenWidth/2 - (resultsText.size()/2 * fontSize / 2), screenHeight/2 + verticalOffset, 0)));
	results->AddComponent(new TextComponent(resultsText, font, SDL_Color{ 255,0,0 }));
	results->AddComponent(new RenderComponent());
	scene->Add(results);

	verticalOffset += fontSize;

	std::string shotsFiredText = "SHOTS FIRED:    ";
	auto shotsFired = std::make_shared<GameObject>("shotsFired");
	shotsFired->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2 - (shotsFiredText.size() / 2 * fontSize / 2), screenHeight / 2 + verticalOffset, 0)));
	shotsFired->AddComponent(new TextComponent(shotsFiredText +"     "+ std::to_string(shots), font, SDL_Color{255,255,0}));
	shotsFired->AddComponent(new RenderComponent());
	scene->Add(shotsFired);

	verticalOffset += fontSize;

	std::string hitsNumberText = "NUMBER OF HITS:  ";
	auto hitsNumber = std::make_shared<GameObject>("hitsNumber");
	hitsNumber->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2 - (hitsNumberText.size() / 2 * fontSize / 2), screenHeight / 2 + verticalOffset, 0)));
	hitsNumber->AddComponent(new TextComponent(hitsNumberText + std::to_string(hits), font, SDL_Color{ 255,255,0 }));
	hitsNumber->AddComponent(new RenderComponent());
	scene->Add(hitsNumber);

	verticalOffset += fontSize;

	std::string hitMissRatioText = "HIT-MISS RATIO: %";
	auto hitMissRatio = std::make_shared<GameObject>("hitMissRatio");
	hitMissRatio->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2 - (hitMissRatioText.size() / 2 * fontSize / 2), screenHeight / 2 + verticalOffset, 0)));
	hitMissRatio->AddComponent(new TextComponent(hitMissRatioText + std::to_string(ratio), font, SDL_Color{ 255,255,255 }));
	hitMissRatio->AddComponent(new RenderComponent());
	scene->Add(hitMissRatio);

}

void SceneLoader::CleanUp() const
{
	EnemyManager::GetInstance().CleanUp();
	CollisionManager::GetInstance().CleanUp();
	SceneManager::GetInstance().GetCurrentScene()->ClearScene();
	LevelManager::GetInstance().ResetLevelManager();
	RocketManager::GetInstance().ResetStatistics();
	SceneManager::GetInstance().GetCurrentScene()->ClearScene();//???
}
