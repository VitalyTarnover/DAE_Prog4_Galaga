#include "MiniginPCH.h"
#include "RocketManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RocketMovementComponent.h"
#include "GameObject.h"
#include "Texture2DComponent.h"

void RocketManager::ReduceActiveRocketsNumber()
{
	--m_ActiveRocketsNumber;
}

void RocketManager::SpawnRocket(bool movesUp) //movesUp also means it was shot by a player
{
	if (movesUp && (m_ActiveRocketsNumber < m_AllowedRocketsNumber))//TODO: Set for 2 players
	{
		glm::vec3 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

		float verticalOffset = 20.f;


		auto rocket = std::make_shared<GameObject>("Rocket");
		rocket->AddComponent(new TransformComponent(glm::vec3(playerPos.x, playerPos.y - verticalOffset, 0)));
		rocket->AddComponent(new Texture2DComponent("Rocket.png", scene->GetSceneScale()));
		rocket->AddComponent(new RocketMovementComponent(true, 250));
		scene->Add(rocket);

		++m_ActiveRocketsNumber;
	}
	
}
