#include "MiniginPCH.h"
#include "RocketManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RocketMovementComponent.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "CollisionManager.h"
#include "RenderComponent.h"
#include "PlayerHealthComponent.h"

void RocketManager::ReduceActiveRocketsNumber()
{
	--m_ActiveRocketsNumber;
}

void RocketManager::ReduceActiveEnemyRocketsNumber()
{
	--m_ActiveEnemyRocketsNumber;
}

void RocketManager::SpawnPlayerRocket() //mby pass index for P1/P2
{
	if (m_ActiveRocketsNumber < m_AllowedRocketsNumber)//TODO: Set for 2 players
	{
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<PlayerHealthComponent>()->IsAlive())
		{
			glm::vec3 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

			auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

			float verticalOffset = 20.f;


			auto rocket = std::make_shared<GameObject>("Rocket");
			rocket->AddComponent(new TransformComponent(glm::vec3(playerPos.x, playerPos.y - verticalOffset, 0)));
			rocket->AddComponent(new RenderComponent());
			rocket->AddComponent(new Texture2DComponent("Rocket.png", scene->GetSceneScale()));
			rocket->AddComponent(new RocketMovementComponent(true, 250));
			scene->Add(rocket);
			CollisionManager::GetInstance().AddGameObjectForCheck(rocket);

			++m_ActiveRocketsNumber;
		}
		
	}
		
}

void RocketManager::SpawnEnemyRocket(const glm::vec3& enemyPos)
{
	//if (m_ActiveEnemyRocketsNumber < m_AllowedEnemyRocketsNumber)//TODO: allowed is literally level
	{

		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

		float verticalOffset = 20.f;

		auto rocket = std::make_shared<GameObject>("Rocket");
		//rocket->AddComponent(new TransformComponent(glm::vec3(enemyPos.x, enemyPos.y + verticalOffset, 0)));
		rocket->AddComponent(new TransformComponent(glm::vec3(enemyPos.x, enemyPos.y + verticalOffset, 0)));
		rocket->AddComponent(new RenderComponent());
		rocket->AddComponent(new Texture2DComponent("Rocket.png", scene->GetSceneScale()));
		rocket->AddComponent(new RocketMovementComponent(false, 250));
		scene->Add(rocket);
		CollisionManager::GetInstance().AddGameObjectForCheck(rocket);

		++m_ActiveEnemyRocketsNumber;
	}

}
