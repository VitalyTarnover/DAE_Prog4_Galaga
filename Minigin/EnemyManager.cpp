#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "SystemTime.h"

#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

#include "BaseEnemyMovementComponent.h"
#include "EnemyFlyInMovement.h"

#include "CollisionManager.h"

void EnemyManager::SpawnEnemies(const std::vector<glm::vec2>& posInFormation)
{
	//set state building formation
	m_BeesPosInFormation = posInFormation;
	m_NumberOfEnemies += int(posInFormation.size());
}

void EnemyManager::Update()
{
	//building formation
	
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
				testEnemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() / 10 * 8, dae::SceneManager::GetInstance().GetScreenHeight() / 3, 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
				testEnemyShip->AddComponent(new Texture2DComponent("Bee.png", scene->GetSceneScale()));
				testEnemyShip->AddComponent(new SpriteAnimComponent(2));
				testEnemyShip->AddComponent(new EnemyFlyInMovement(m_BeesPosInFormation.back()));
				scene->Add(testEnemyShip);
				CollisionManager::GetInstance().AddGameObjectForCheck(true, testEnemyShip);
			}
			m_BeesPosInFormation.pop_back();
			m_SpawnTimer = 0;

		}
		else m_SpawnTimer += SystemTime::GetInstance().GetDeltaTime();
	}

}

