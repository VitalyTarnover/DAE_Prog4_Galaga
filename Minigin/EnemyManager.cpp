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
				auto testEnemyShip = std::make_shared<GameObject>("TestEnemy");
				testEnemyShip->AddComponent(new TransformComponent(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() / 2, -20, 0), 13.f, 10.f, scene->GetSceneScale(), scene->GetSceneScale()));
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

