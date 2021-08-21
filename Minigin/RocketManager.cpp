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
#include "Event.h"

void RocketManager::ReduceActiveRocketsNumber()
{
	--m_ActiveRocketsNumber;
}

void RocketManager::ReduceActiveEnemyRocketsNumber()
{
	--m_ActiveEnemyRocketsNumber;
}


void RocketManager::SpawnPlayerRocket(int playerIndex)
{
if (m_ActiveRocketsNumber < m_AllowedRocketsNumber)
	{
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex)->GetComponent<PlayerHealthComponent>()->IsAlive())
		{
			glm::vec3 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex)->GetComponent<TransformComponent>()->GetCenterPosition();

			auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

			float verticalOffset = 20.f;

			auto rocket = std::make_shared<GameObject>("Rocket");
			rocket->AddComponent(new TransformComponent(glm::vec3(playerPos.x, playerPos.y - verticalOffset, 0),3,8, scene->GetSceneScale()));
			rocket->AddComponent(new RenderComponent());
			rocket->AddComponent(new Texture2DComponent("Rocket.png", scene->GetSceneScale()));
			rocket->AddComponent(new RocketMovementComponent(true, 250));
			rocket->GetComponent<RocketMovementComponent>()->SetOwner(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex));
			scene->Add(rocket);
			CollisionManager::GetInstance().AddGameObjectForCheck(rocket);

			++m_ActiveRocketsNumber;
			++m_NumberOfShotsFired;

			m_EventShotFired.Notify(nullptr, "playerShot");
		}
	}
}


void RocketManager::SpawnEnemyRocket(const glm::vec3& enemyPos) const
{
		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

		float verticalOffset = 20.f;

		auto rocket = std::make_shared<GameObject>("EnemyRocket");
		rocket->AddComponent(new TransformComponent(glm::vec3(enemyPos.x, enemyPos.y + verticalOffset, 0)));
		rocket->AddComponent(new RenderComponent());
		rocket->AddComponent(new Texture2DComponent("EnemyRocket.png", scene->GetSceneScale()));
		rocket->AddComponent(new RocketMovementComponent(false, 250));
		scene->Add(rocket);
		CollisionManager::GetInstance().AddGameObjectForCheck(rocket);
		m_EventShotFired.Notify(nullptr, "enemyShot");
}

void RocketManager::SpawnEnemyRocket()//spawns a rocket from player controlled bird
{
	if (m_ActiveEnemyRocketsNumber < m_AllowedEnemyRocketsNumber)
	{
		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

		auto player = scene->GetPlayer(1);

		if (player)
		{
			glm::vec3 currentPosition = player->GetComponent<TransformComponent>()->GetTransform().GetPosition();

			if (currentPosition.y > 0 && currentPosition.y < dae::SceneManager::GetInstance().GetScreenHeight() &&
				currentPosition.x > 0 && currentPosition.x < dae::SceneManager::GetInstance().GetScreenWidth())
			{
				glm::vec3 playerPos = scene->GetPlayer(1)->GetComponent<TransformComponent>()->GetCenterPosition();

				float verticalOffset = 20.f;

				auto rocket = std::make_shared<GameObject>("EnemyRocket");
				//rocket->AddComponent(new TransformComponent(glm::vec3(enemyPos.x, enemyPos.y + verticalOffset, 0)));
				rocket->AddComponent(new TransformComponent(glm::vec3(playerPos.x, playerPos.y + verticalOffset, 0)));
				rocket->AddComponent(new RenderComponent());
				rocket->AddComponent(new Texture2DComponent("EnemyRocket.png", scene->GetSceneScale()));
				rocket->AddComponent(new RocketMovementComponent(false, 250, true));
				scene->Add(rocket);
				CollisionManager::GetInstance().AddGameObjectForCheck(rocket);
				++m_ActiveEnemyRocketsNumber;

				m_EventShotFired.Notify(nullptr, "enemyShot");
			}
			
		}
	}
	
	
}

void RocketManager::Reset()
{
	m_NumberOfShotsFired = 0;
	m_NumberOfHits = 0;
	m_ActiveRocketsNumber = 0;
	m_ActiveEnemyRocketsNumber = 0;

	m_EventShotFired.ResetHandlers();
}

void RocketManager::SetAudioEventHandler(const std::shared_ptr<IEventHandler>& audioEventHandler)
{
	m_EventShotFired.AddHandler(audioEventHandler);
}

void RocketManager::ShotHit()
{
	++m_NumberOfHits;
}
