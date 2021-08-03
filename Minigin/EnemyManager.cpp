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

EnemyManager::~EnemyManager()
{
	m_Enemies.clear();
}

void EnemyManager::SpawnEnemies(const std::vector<std::vector<int>>& beeInfo, 
	const std::vector<std::vector<int>>& bfInfo, 
	const std::vector<std::vector<int>>& birdInfo)
{
	//set state building formation
	m_BuildingFormation = true;
	
	m_BeeInfo = beeInfo;
	m_BFInfo = bfInfo;
	m_BirdInfo = birdInfo;

	m_NumberOfEnemiesNotInPosition = int(m_BeeInfo.size() + m_BFInfo.size() + m_BirdInfo.size() );
	m_NumberOfEnemiesAlive = m_NumberOfEnemiesNotInPosition;
	
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
				//enemyShip->AddComponent(new EnemyFlyInMovement(m_EnemiesPosInFormation.back()));
				enemyShip->AddComponent(new BeeMovementComponent
				(275.f,glm::vec2( (screenWidth / m_BeeInfo.back()[1]) * m_BeeInfo.back()[0], screenHeight / 14 * (5 + m_BeeInfo.back()[2]) )));
				scene->Add(enemyShip);
				m_Enemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(true, enemyShip);
				m_BeeInfo.pop_back();
			}
			else if (m_BFInfo.size() > 0)
			{
				auto enemyShip = std::make_shared<GameObject>("BF");
				enemyShip->AddComponent(new TransformComponent(glm::vec3(-100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
				enemyShip->AddComponent(new RenderComponent());
				enemyShip->AddComponent(new Texture2DComponent("Butterfly.png", scene->GetSceneScale()));
				enemyShip->AddComponent(new SpriteAnimComponent(2));
				//enemyShip->AddComponent(new EnemyFlyInMovement(m_EnemiesPosInFormation.back()));
				enemyShip->AddComponent(new BFMovementComponent(275.f, 
					m_BFInfo.back()[3], glm::vec2((screenWidth / m_BFInfo.back()[1]) * m_BFInfo.back()[0], screenHeight / 14 * (3 + m_BFInfo.back()[2]))));
				scene->Add(enemyShip);
				m_Enemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(true, enemyShip);
				m_BFInfo.pop_back();
			}
			else if (m_BirdInfo.size() > 0)
			{
				auto enemyShip = std::make_shared<GameObject>("Bird");
				enemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() + 100, dae::SceneManager::GetInstance().GetScreenHeight(), 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
				enemyShip->AddComponent(new RenderComponent());
				enemyShip->AddComponent(new Texture2DComponent("Bird.png", scene->GetSceneScale()));
				enemyShip->AddComponent(new SpriteAnimComponent(4));
				//enemyShip->AddComponent(new EnemyFlyInMovement(m_EnemiesPosInFormation.back()));
				enemyShip->AddComponent(new BirdMovementComponent(275.f, 
					m_BirdInfo.back()[2], glm::vec2( (screenWidth / m_BirdInfo.back()[1]) * m_BirdInfo.back()[0], screenHeight / 9 )));
				scene->Add(enemyShip);
				m_Enemies.push_back(enemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(true, enemyShip);
				m_BirdInfo.pop_back();
			}

			//m_EnemiesPosInFormation.pop_back();
			m_SpawnTimer = 0;
		}
		else m_SpawnTimer += SystemTime::GetInstance().GetDeltaTime();


		if (m_NumberOfEnemiesNotInPosition == 0 && m_CurrentStepNumber == 0)//here is that check you are looking for
		{
			for (size_t i = 0; i < m_Enemies.size(); i++)
			{
				m_Enemies[i]->GetComponent<BaseEnemyMovementComponent>()->Switch();//works
			}
			m_BuildingFormation = false;
		}

	}
	
	if (m_Enemies.size() > 0)
	{
		ShootersShoot();
		if(!m_BuildingFormation && !m_WaitingForPlayerToRespawn) SendRandomEnemyToAttack();
	
		CalculatePatrolSteps();
	}
	
}

void EnemyManager::ResetEnemies()
{
	m_Enemies.clear();//TODO: clear all of them!
}

void EnemyManager::AnEnemyReachedPositionInFormation()
{
	--m_NumberOfEnemiesNotInPosition;

}

void EnemyManager::SetWaitingForPlayerToRespawn(bool waiting)
{
	m_WaitingForPlayerToRespawn = waiting;
}

void EnemyManager::ShootersShoot()
{
	if (m_EnemyShooters.size() > 0)
	{
		if (m_ShootingTimer > 0) m_ShootingTimer -= SystemTime::GetInstance().GetDeltaTime();
		else
		{
			m_EnemyShooters.back()->GetComponent<BaseEnemyMovementComponent>()->ShootARocket();
			m_EnemyShooters.pop_back();

			m_ShootingTimer = (rand() % m_ShootingTimeMax + m_ShootingTimeMin)/100.f;
		}
	}
}

//void EnemyManager::OnEvent()
//{
//	AnEnemyReachedPositionInFormation();
//}

void EnemyManager::DeleteEnemy(const std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0; i < m_Enemies.size(); ++i)
	{
		if (m_Enemies[i] == gameObject)
		{
			m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), m_Enemies[i]), m_Enemies.end());
			return;
		}
	}

}

void EnemyManager::SendRandomEnemyToAttack()
{
	if (m_DiveDownTimer > 0) m_DiveDownTimer -= SystemTime::GetInstance().GetDeltaTime();
	else
	{
		int extraEnemiesChance = rand() % 1 + 1;//TODO: instead of 3 take current level...

		for (size_t i = extraEnemiesChance; i > 0; --i)
		{
			int randomEnemyIndex = rand() % m_Enemies.size();
			BaseEnemyMovementComponent* enemyMovement = m_Enemies[randomEnemyIndex]->GetComponent<BaseEnemyMovementComponent>();


			int birdCompanionIndex = enemyMovement->GetBirdCompanionIndex();

			if (birdCompanionIndex == -1)
			{
				enemyMovement->Switch();

				int chanceToShootModifier = 3;

				int chanceToShoot = rand() % chanceToShootModifier + 1;
				if (chanceToShoot == chanceToShootModifier)
				{
					m_EnemyShooters.push_back(m_Enemies[randomEnemyIndex]);
				}
			}
			else
			{
				for (size_t j  = 0; j < m_Enemies.size(); j++)
				{
					BaseEnemyMovementComponent* nextEnemyMovement = m_Enemies[j]->GetComponent<BaseEnemyMovementComponent>();

					if (birdCompanionIndex == nextEnemyMovement->GetBirdCompanionIndex())
						nextEnemyMovement->Switch();

				}
			}
		}

		m_DiveDownTimer = m_DiveDownTime;
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
	

