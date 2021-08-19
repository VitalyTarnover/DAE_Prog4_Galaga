#include "MiniginPCH.h"
#include "RocketMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RocketManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "SceneLoader.h"

RocketMovementComponent::RocketMovementComponent(bool movesUp, float speed, bool enemyPlayerShot) //movesUp also means it was shot by a player
	:m_MovesUp{ movesUp }
	,m_Speed {speed}
	,m_Direction{0,0}
	,m_EnemyPlayerShot{enemyPlayerShot}
{
}

RocketMovementComponent::~RocketMovementComponent()
{
	if (m_MovesUp) RocketManager::GetInstance().ReduceActiveRocketsNumber();
	else
	{
		if (m_EnemyPlayerShot)
		{
			RocketManager::GetInstance().ReduceActiveEnemyRocketsNumber();
		}
	}
}

void RocketMovementComponent::Update()
{
	if (!m_IsInitialized)
	{
		if (!m_MovesUp)
		{
			int playerIndex = 0;
			if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Coop)playerIndex = rand() % 2;

			glm::vec2 distance = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex)->
				GetComponent<TransformComponent>()->GetCenterPosition() - m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition();
			
			m_Direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));

		}
		m_IsInitialized = true;
	}

	Move();
	OutsideBordersCheck();
}

void RocketMovementComponent::Move()
{
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 oldPosition = trc->GetTransform().GetPosition();
	
	if(m_MovesUp)trc->SetPosition(glm::vec3(oldPosition.x, oldPosition.y - (m_Speed * SystemTime::GetInstance().GetDeltaTime()), 0));
	else
	{
		glm::vec2 translation = m_Direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

		trc->SetPosition(glm::vec3(oldPosition.x + translation.x, oldPosition.y + translation.y, 0));
	}

}

void RocketMovementComponent::OutsideBordersCheck()
{
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 currentPosition = trc->GetTransform().GetPosition();

	if (currentPosition.y < 0 || currentPosition.y > dae::SceneManager::GetInstance().GetScreenHeight())
	{
		m_pGameObject->SetMarkedForDelete(true);
	}
}
