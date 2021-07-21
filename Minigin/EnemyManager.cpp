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
#include "EnemyFlyInMovement.h"

#include "CollisionManager.h"

EnemyManager::~EnemyManager()
{
	m_Enemies.clear();
}

void EnemyManager::SpawnEnemies(const std::vector<glm::vec2>& posInFormation)
{
	//set state building formation
	m_BuildingFormation = true;
	m_BeesPosInFormation = posInFormation;
	m_NumberOfEnemiesNotInPosition = int(posInFormation.size());
	m_NumberOfEnemiesAlive = m_NumberOfEnemiesNotInPosition;
	//TODO: okay.. so check how enemies patrol before and after getting everyone in formation. I think it should be synchronized here.
	
}

void EnemyManager::Update()
{
	//building formation
	if (m_BuildingFormation)
	{
		if (m_BeesPosInFormation.size() > 0)
		{
			if (m_SpawnTimer >= m_SpawnTime)
			{
				auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
				{
					//TODO: make spawning for all types of enemies. It will be a check "how many enemies are left to spawn" 
					//Just check the size of m_BeesPosInFormation. If it is more than 24 -> spawn bees
					//If it is less than 24 but more than 4 -> spawn BFs
					//If it is less than -> spawn birds
					//We are limitied to this exact amount of enemies per stage now, but just make different formations and increase speed or something
					auto testEnemyShip = std::make_shared<GameObject>("TestEnemy");
					testEnemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() / 2, -100, 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
					testEnemyShip->AddComponent(new Texture2DComponent("Bee.png", scene->GetSceneScale()));
					testEnemyShip->AddComponent(new SpriteAnimComponent(2));
					//testEnemyShip->AddComponent(new EnemyFlyInMovement(m_BeesPosInFormation.back()));
					testEnemyShip->AddComponent(new BeeMovementComponent(200.f, m_BeesPosInFormation.back()));
					scene->Add(testEnemyShip);
					m_Enemies.push_back(testEnemyShip);
					CollisionManager::GetInstance().AddGameObjectForCheck(true, testEnemyShip);
	
				}
				m_BeesPosInFormation.pop_back();
				m_SpawnTimer = 0;
	
			}
			else m_SpawnTimer += SystemTime::GetInstance().GetDeltaTime();
		}
	
		if (m_NumberOfEnemiesNotInPosition <= 0 && m_CurrentStepNumber <= 0)//here is that check you are looking for
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
		if(!m_BuildingFormation) SendRandomEnemyToAttack();

		CalculatePatrolSteps();
	}
	
}

void EnemyManager::ResetEnemies()
{
	m_Enemies.clear();
}

void EnemyManager::AnEnemyReachedPositionInFormation()
{
	--m_NumberOfEnemiesNotInPosition;
}

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
		int randomEnemyIndex = rand() % m_Enemies.size();
		m_Enemies[randomEnemyIndex]->GetComponent<BaseEnemyMovementComponent>()->Switch();

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
	

