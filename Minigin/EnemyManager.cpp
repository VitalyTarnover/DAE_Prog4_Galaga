#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "SystemTime.h"

#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

#include "BeeBehaviorComponent.h"
#include "BFBehaviorComponent.h"
#include "BirdBehaviorComponent.h"
#include "RenderComponent.h"
#include "CollisionManager.h"
#include "SceneLoader.h"

EnemyManager::~EnemyManager()
{
	m_pEnemies.clear();
	m_pEnemyShooters.clear();
}

void EnemyManager::SpawnEnemies(const std::vector<std::vector<int>>& beeInfo,
	const std::vector<std::vector<int>>& bfInfo,
	const std::vector<std::vector<int>>& birdInfo,
	const std::vector<std::shared_ptr<IEventHandler>>& handlers)
{
	//set state building formation
	m_BuildingFormation = true;

	m_BeeInfo = beeInfo;
	m_BFInfo = bfInfo;
	m_BirdInfo = birdInfo;

	m_NumberOfEnemiesNotInPosition = int(m_BeeInfo.size() + m_BFInfo.size() + m_BirdInfo.size());
	m_NumberOfEnemiesAlive = m_NumberOfEnemiesNotInPosition;

	m_PanicMode = false;

	if (!m_AudioEventHandler) m_AudioEventHandler = handlers[1];//audio
	if (!m_ScoreEventHandler) m_ScoreEventHandler = handlers[3];//score
	m_EventLevelCleared.ResetHandlers();
	m_EventLevelCleared.AddHandler(handlers[2]);//level cleared

	m_RespawnWaitingTimer = 0.f;
	m_SpawnTimer = 0.f;
	m_DiveDownTimer = 0.f;
	m_StepTimer = 0.f;

}

void EnemyManager::Update()
{

	EnemiesHandler();

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
	
	m_pEnemyShooters.clear();

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
			if (m_DiveDownTimer < m_DiveDownTime) m_DiveDownTimer += SystemTime::GetInstance().GetDeltaTime();
			else
			{
				if (int(m_pEnemies.size()) <= m_PanicEnemiesNumber)
				{
					m_PanicMode = true;

					for (auto enemy : m_pEnemies)
					{
						BaseEnemyBehaviorComponent* movementComponent = enemy->GetComponent<BaseEnemyBehaviorComponent>();
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
						BaseEnemyBehaviorComponent* enemyMovement = m_pEnemies[randomEnemyIndex]->GetComponent<BaseEnemyBehaviorComponent>();

						if (!enemyMovement->GetIsAttacking())
						{
							int birdCompanionIndex = enemyMovement->GetBirdCompanionIndex();

							if (birdCompanionIndex == -1)
							{
								enemyMovement->Switch();
							}
							else
							{
								BirdBehaviorComponent* birdMovementComponent = m_pEnemies[randomEnemyIndex]->GetComponent<BirdBehaviorComponent>();

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
											BFBehaviorComponent* enemyCompanionMovement = m_pEnemies[j]->GetComponent<BFBehaviorComponent>();
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
									BFBehaviorComponent* butterflyMovement = m_pEnemies[randomEnemyIndex]->GetComponent<BFBehaviorComponent>();
									butterflyMovement->SetIsWithBird(false);
									butterflyMovement->Switch();
								}
							}
						}
					}
					m_DiveDownTimer = 0;
				}
			}
		}
	}
	else if (m_WaitingForPlayerToRespawn)
	{
		m_PanicMode = false;
		for (auto enemy : m_pEnemies)
		{
			BaseEnemyBehaviorComponent* movementComponent = enemy->GetComponent<BaseEnemyBehaviorComponent>();
			movementComponent->SetPanic(false);
			movementComponent->Switch();
		}
	}

}

void EnemyManager::RespawnWaitingHandler()
{
	if (!m_BuildingFormation)
	{
		for (size_t i = 0; i < m_pEnemies.size(); i++)
		{
			if (m_pEnemies[i]->GetComponent<BaseEnemyBehaviorComponent>()->GetIsAttacking()) return;
		}

		if (m_RespawnWaitingTimer < m_RespawnWaitingTime) m_RespawnWaitingTimer += SystemTime::GetInstance().GetDeltaTime();
		else
		{
			m_WaitingForPlayerToRespawn = false;
			m_RespawnWaitingTimer = 0;
		}
	}
}

void EnemyManager::EnemiesHandler()
{
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
				enemyShip->AddComponent(new BeeBehaviorComponent
				(275.f, glm::vec2((screenWidth / m_BeeInfo.back()[1]) * m_BeeInfo.back()[0], screenHeight / 14 * (5 + m_BeeInfo.back()[2]))));
				enemyShip->GetComponent<BeeBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
				enemyShip->GetComponent<BeeBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_AudioEventHandler);
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
				enemyShip->AddComponent(new BFBehaviorComponent(275.f,
					m_BFInfo.back()[3], glm::vec2((screenWidth / m_BFInfo.back()[1]) * m_BFInfo.back()[0], screenHeight / 14 * (3 + m_BFInfo.back()[2]))));
				scene->Add(enemyShip);
				enemyShip->GetComponent<BFBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
				enemyShip->GetComponent<BFBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_AudioEventHandler);
				m_pEnemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
				m_BFInfo.pop_back();
			}
			else if (m_BirdInfo.size() > 0)
			{
				if (m_BirdInfo.size() == 2 && SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Versus)
				{
					if (scene->GetPlayer(1) == nullptr)
					{
						auto enemyShip = std::make_shared<GameObject>("Bird");
						enemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() + 100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 15.f, 16.f, scene->GetSceneScale(), scene->GetSceneScale()));
						enemyShip->AddComponent(new RenderComponent());
						enemyShip->AddComponent(new SpriteAnimComponent(4));
						enemyShip->AddComponent(new Texture2DComponent("Bird2.png", scene->GetSceneScale()));
						enemyShip->AddComponent(new BirdBehaviorComponent(275.f,
							m_BirdInfo.back()[2], glm::vec2((screenWidth / m_BirdInfo.back()[1]) * m_BirdInfo.back()[0], screenHeight / 9), true));
						scene->Add(enemyShip);
						scene->AddPlayer(enemyShip);
						enemyShip->GetComponent<BirdBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
						enemyShip->GetComponent<BirdBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_AudioEventHandler);

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
					enemyShip->AddComponent(new BirdBehaviorComponent(275.f,
						m_BirdInfo.back()[2], glm::vec2((screenWidth / m_BirdInfo.back()[1]) * m_BirdInfo.back()[0], screenHeight / 9)));
					scene->Add(enemyShip);
					m_pEnemies.push_back(enemyShip);
					enemyShip->GetComponent<BirdBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_ScoreEventHandler);
					enemyShip->GetComponent<BirdBehaviorComponent>()->GetEventEnemyKilledHandler()->AddHandler(m_AudioEventHandler);


					CollisionManager::GetInstance().AddGameObjectForCheck(enemyShip);
				}
				m_BirdInfo.pop_back();

			}
			m_SpawnTimer = 0;
		}
		else m_SpawnTimer += SystemTime::GetInstance().GetDeltaTime();


		if (m_NumberOfEnemiesNotInPosition == 0 && m_CurrentStepNumber == 0)//here is that check you are looking for
		{
			for (size_t i = 0; i < m_pEnemies.size(); i++)
			{
				m_pEnemies[i]->GetComponent<BaseEnemyBehaviorComponent>()->Switch();//works
			}

			if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Versus)
			{
				auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
				if (player2)player2->GetComponent<BirdBehaviorComponent>()->Switch();
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
}

void EnemyManager::RandomEnemyShot() const
{
	if (m_pEnemies.size() > 0)
	{
		int randomEnemyIndex = rand() % m_pEnemies.size();

		int chanceRange = 200;
		int diffcultyLevelWeight = 60;

		int chanceToShootModifier = chanceRange - (m_CurrentDifficultyLevel * diffcultyLevelWeight);

		int chanceToShoot = rand() % chanceToShootModifier + 1;
		if (chanceToShoot == chanceToShootModifier)
		{
			float enemyYPos = m_pEnemies[randomEnemyIndex]->GetComponent<TransformComponent>()->GetCenterPosition().y;
			if (enemyYPos < dae::SceneManager::GetInstance().GetScreenHeight()/3 && enemyYPos > 0)
			m_pEnemies[randomEnemyIndex]->GetComponent<BaseEnemyBehaviorComponent>()->ShootARocket();
		}
	}
}


int EnemyManager::GetPatrolStep() const
{
	return m_CurrentStepNumber;
}

void EnemyManager::CalculatePatrolSteps()
{
	//Patrolling in formation
	if (m_StepTimer > 0) m_StepTimer -= SystemTime::GetInstance().GetDeltaTime();
	else
	{
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
	

