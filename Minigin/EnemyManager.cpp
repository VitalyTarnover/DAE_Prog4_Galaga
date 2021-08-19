#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "SystemTime.h"

#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

//#include "BaseEnemyMovementComponent.h"
#include "BeeMovementComponent.h"
#include "BFMovementComponent.h"
#include "BirdMovementComponent.h"
#include "RenderComponent.h"
#include "CollisionManager.h"
#include "SceneLoader.h"

EnemyManager::~EnemyManager()
{
	m_pEnemies.clear();
}

void EnemyManager::SpawnEnemies(const std::vector<std::vector<int>>& beeInfo,
	const std::vector<std::vector<int>>& bfInfo,
	const std::vector<std::vector<int>>& birdInfo,
	std::vector<std::shared_ptr<IEventHandler>> handlers)
{
	//set state building formation
	m_BuildingFormation = true;

	m_BeeInfo = beeInfo;
	m_BFInfo = bfInfo;
	m_BirdInfo = birdInfo;

	m_NumberOfEnemiesNotInPosition = int(m_BeeInfo.size() + m_BFInfo.size() + m_BirdInfo.size());
	m_NumberOfEnemiesAlive = m_NumberOfEnemiesNotInPosition;

	m_PanicMode = false;

	m_DiveDownTimer = m_DiveDownTime;

	if (!m_ScoreEventHandler) m_ScoreEventHandler = handlers[0];
	//m_EventLevelCleared = std::make_shared<Event>();
	m_EventLevelCleared.ResetHandlers();
	m_EventLevelCleared.AddHandler(handlers[1]);

	m_RespawnWaitingTimer = 0.f;
	m_SpawnTimer = 0.f;
	m_RespawnWaitingTimer = 0.f;
	m_DiveDownTimer = 0.f;
	m_StepTimer = 0.f;

}

void EnemyManager::Update()
{
	//building formation
	if (m_BuildingFormation)
	{
		int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
		int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

		if (m_SpawnTimer >= m_SpawnTime)
		{
			auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
			
			if (m_BeeInfo.size() > 0)
			{
				auto enemyShip = std::make_shared<GameObject>("Bee");
				enemyShip->AddComponent(new TransformComponent(glm::vec3(screenWidth / 2, -100, 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
				enemyShip->AddComponent(new RenderComponent());
				enemyShip->AddComponent(new Texture2DComponent("Bee.png", scene->GetSceneScale()));
				enemyShip->AddComponent(new SpriteAnimComponent(2));
				//enemyShip->AddComponent(new EnemyFlyInMovement(m_pEnemiesPosInFormation.back()));
				enemyShip->AddComponent(new BeeMovementComponent
				(275.f,glm::vec2( (screenWidth / m_BeeInfo.back()[1]) * m_BeeInfo.back()[0], screenHeight / 14 * (5 + m_BeeInfo.back()[2]) )));
				enemyShip->GetComponent<BeeMovementComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
				scene->Add(enemyShip);
				m_pEnemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
				m_BeeInfo.pop_back();
			}
			else if (m_BFInfo.size() > 0)
			{
				auto enemyShip = std::make_shared<GameObject>("BF");
				enemyShip->AddComponent(new TransformComponent(glm::vec3(-100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
				enemyShip->AddComponent(new RenderComponent());
				enemyShip->AddComponent(new Texture2DComponent("Butterfly.png", scene->GetSceneScale()));
				enemyShip->AddComponent(new SpriteAnimComponent(2));
				//enemyShip->AddComponent(new EnemyFlyInMovement(m_pEnemiesPosInFormation.back()));
				enemyShip->AddComponent(new BFMovementComponent(275.f, 
					m_BFInfo.back()[3], glm::vec2((screenWidth / m_BFInfo.back()[1]) * m_BFInfo.back()[0], screenHeight / 14 * (3 + m_BFInfo.back()[2]))));
				scene->Add(enemyShip);
				enemyShip->GetComponent<BFMovementComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
				m_pEnemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
				m_BFInfo.pop_back();
			}
			else if (m_BirdInfo.size() > 0)
			{
				if (m_BirdInfo.size() == 2 && SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Versus )
				{
					if (scene->GetPlayer(1) == nullptr)
					{
						auto enemyShip = std::make_shared<GameObject>("Bird");
						enemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() + 100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
						enemyShip->AddComponent(new RenderComponent());
						enemyShip->AddComponent(new SpriteAnimComponent(4));
						enemyShip->AddComponent(new Texture2DComponent("Bird2.png", scene->GetSceneScale()));
						enemyShip->AddComponent(new BirdMovementComponent(275.f,
							m_BirdInfo.back()[2], glm::vec2((screenWidth / m_BirdInfo.back()[1]) * m_BirdInfo.back()[0], screenHeight / 9), true));
						scene->Add(enemyShip);
						scene->AddPlayer(enemyShip);
						enemyShip->GetComponent<BirdMovementComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);

						CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
					}
					else --m_NumberOfEnemiesNotInPosition;
				}
				else
				{
					auto enemyShip = std::make_shared<GameObject>("Bird");
					enemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() + 100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
					enemyShip->AddComponent(new RenderComponent());
					enemyShip->AddComponent(new SpriteAnimComponent(4));
					enemyShip->AddComponent(new Texture2DComponent("Bird.png", scene->GetSceneScale()));
					enemyShip->AddComponent(new BirdMovementComponent(275.f,
						m_BirdInfo.back()[2], glm::vec2((screenWidth / m_BirdInfo.back()[1]) * m_BirdInfo.back()[0], screenHeight / 9)));
					scene->Add(enemyShip);
					m_pEnemies.push_back(enemyShip);
					enemyShip->GetComponent<BirdMovementComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);

					CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
				}
				m_BirdInfo.pop_back();

			}
			//m_pEnemiesPosInFormation.pop_back();
			m_SpawnTimer = 0;
		}
		else m_SpawnTimer += SystemTime::GetInstance().GetDeltaTime();


		if (m_NumberOfEnemiesNotInPosition == 0 && m_CurrentStepNumber == 0)//here is that check you are looking for
		{
			for (size_t i = 0; i < m_pEnemies.size(); i++)
			{
				m_pEnemies[i]->GetComponent<BaseEnemyMovementComponent>()->Switch();//works
			}

			if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Versus)
			{
				auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
				if (player2)player2->GetComponent<BirdMovementComponent>()->Switch();
			}


			m_BuildingFormation = false;
		}

	}
	else
	{
		if (m_pEnemies.size() > 0)
		{
			if (!m_BuildingFormation) SendRandomEnemyToAttack();
		}

		if (m_pEnemies.size() <= 0 && m_NumberOfEnemiesNotInPosition <= 0)
		{
			m_BuildingFormation = true;
			++m_CurrentDifficultyLevel;
			m_EventLevelCleared.Notify(nullptr, "LevelCleared");
		}
	}

	CalculatePatrolSteps();

	if (!m_WaitingForPlayerToRespawn) RandomEnemyShot();
	else RespawnWaitingHandler();
	
}

void EnemyManager::CleanUp()
{
	m_CurrentDifficultyLevel = 1;
	m_WaitingForPlayerToRespawn = false;

	m_pEnemies.clear();

	m_BeeInfo.clear();
	m_BFInfo.clear();
	m_BirdInfo.clear();
	
	m_EnemyShooters.clear();

	m_EventLevelCleared.ResetHandlers();
}



void EnemyManager::AnEnemyReachedPositionInFormation()
{
	--m_NumberOfEnemiesNotInPosition;

}

void EnemyManager::SetWaitingForPlayerToRespawn(bool waiting)
{
	m_WaitingForPlayerToRespawn = waiting;
}



void EnemyManager::DeleteEnemy(const std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0; i < m_pEnemies.size(); ++i)
	{
		if (m_pEnemies[i] == gameObject)
		{
			m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[i]), m_pEnemies.end());
			return;
		}
	}

}

void EnemyManager::SendRandomEnemyToAttack()
{
	if (!m_PanicMode)
	{
		if (!m_WaitingForPlayerToRespawn)
		{
			if (m_DiveDownTimer > 0) m_DiveDownTimer -= SystemTime::GetInstance().GetDeltaTime();
			else
			{
				if (m_pEnemies.size() <= m_PanicEnemiesNumber)
				{
					m_PanicMode = true;

					for (auto enemy : m_pEnemies)
					{
						BaseEnemyMovementComponent* movementComponent = enemy->GetComponent<BaseEnemyMovementComponent>();
						movementComponent->SetPanic(true);
						movementComponent->Switch();
					}

				}
				else
				{
					int extraEnemiesChance = rand() % m_CurrentDifficultyLevel + 1;
					for (size_t i = extraEnemiesChance; i > 0; --i)
					{
						int randomEnemyIndex = rand() % m_pEnemies.size();
						BaseEnemyMovementComponent* enemyMovement = m_pEnemies[randomEnemyIndex]->GetComponent<BaseEnemyMovementComponent>();

						if (!enemyMovement->GetIsAttacking())
						{
							int birdCompanionIndex = enemyMovement->GetBirdCompanionIndex();

							if (birdCompanionIndex == -1)
							{
								enemyMovement->Switch();

								
							}
							else
							{
								BirdMovementComponent* birdMovementComponent = m_pEnemies[randomEnemyIndex]->GetComponent<BirdMovementComponent>();

								if (birdMovementComponent)//if it is a bird it is either goes solo tractor-attacking or bombing but with 2 butterflies
								{
									int birdAttackType;
									if (!birdMovementComponent->GetHasFighterCaptured()) birdAttackType = rand() % 2;
									else birdAttackType = 0;

									if (birdAttackType == 0)
									{
										birdMovementComponent->SetIsBombing(true);
										birdMovementComponent->Switch();

										for (size_t j = 0; j < m_pEnemies.size(); j++)
										{
											BFMovementComponent* enemyCompanionMovement = m_pEnemies[j]->GetComponent<BFMovementComponent>();
											if (enemyCompanionMovement && birdCompanionIndex == enemyCompanionMovement->GetBirdCompanionIndex())
											{
												enemyCompanionMovement->SetIsWithBird(true);
												enemyCompanionMovement->Switch();
											}
										}
									}
									else
									{
										birdMovementComponent->SetIsBombing(false);
										birdMovementComponent->Switch();
									}

								}
								else//if it is a butterfly -> then it just does its usual attack
								{
									BFMovementComponent* butterflyMovement = m_pEnemies[randomEnemyIndex]->GetComponent<BFMovementComponent>();
									butterflyMovement->SetIsWithBird(false);
									butterflyMovement->Switch();
								}
							}
						}
					}
					m_DiveDownTimer = m_DiveDownTime;
				}

			}
		}

	}
	else if (m_WaitingForPlayerToRespawn)
	{
		m_PanicMode = false;
		for (auto enemy : m_pEnemies)
		{
			BaseEnemyMovementComponent* movementComponent = enemy->GetComponent<BaseEnemyMovementComponent>();
			movementComponent->SetPanic(false);
			movementComponent->Switch();
		}
	}

}

void EnemyManager::RespawnWaitingHandler()
{
		if (m_RespawnWaitingTimer < m_RespawnWaitingTime) m_RespawnWaitingTimer += SystemTime::GetInstance().GetDeltaTime();
		else
		{
			m_WaitingForPlayerToRespawn = false;
			m_RespawnWaitingTimer = 0;
		}
}

void EnemyManager::RandomEnemyShot()
{
	if (m_pEnemies.size() > 0)
	{
		int randomEnemyIndex = rand() % m_pEnemies.size();

		int chanceRange = 200;
		int diffcultyLevelWeight = 25;

		int chanceToShootModifier = chanceRange - (m_CurrentDifficultyLevel * diffcultyLevelWeight);

		int chanceToShoot = rand() % chanceToShootModifier + 1;
		if (chanceToShoot == chanceToShootModifier)
		{
			if (m_pEnemies[randomEnemyIndex]->GetComponent<TransformComponent>()->GetCenterPosition().y < dae::SceneManager::GetInstance().GetScreenHeight()/2)
			m_pEnemies[randomEnemyIndex]->GetComponent<BaseEnemyMovementComponent>()->ShootARocket();
		}
	}
}



int EnemyManager::GetPatrolStep()
{
	return m_CurrentStepNumber;
}

void EnemyManager::CalculatePatrolSteps()
{
	//Patrolling in formation
	if (m_StepTimer > 0) m_StepTimer -= SystemTime::GetInstance().GetDeltaTime();
	else
	{
		//float sceneScale = dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();

		if (m_MovingLeft)
		{
			++m_CurrentStepNumber;
			if (m_CurrentStepNumber >= m_StepsNumber) m_MovingLeft = false;
		}
		else
		{
			--m_CurrentStepNumber;
			if (m_CurrentStepNumber <= 0) m_MovingLeft = true;
		}

		m_StepTimer = m_StepTime;
	}
	
}
	

