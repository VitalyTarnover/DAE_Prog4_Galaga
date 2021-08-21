#include "MiniginPCH.h"
#include "BeeDiveDownState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "BeeBehaviorComponent.h"
#include "InFormationState.h"
#include "EnemyManager.h"
#include "SceneLoader.h"
#include "PlayerHealthComponent.h"

BeeDiveDownState::BeeDiveDownState(float speed)
	:BaseDynamicState()
{
	m_Speed = speed;
	m_CurrentWaypoint = 0;
}

BaseEnemyState* BeeDiveDownState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	if (BeeDiveDown(enemy))
	{
		BeeBehaviorComponent* movementComponent = enemy->GetComponent<BeeBehaviorComponent>();
		
		movementComponent->SetIsAttacking(false);

		if (!movementComponent->GetIsPanicing())return new InFormationState();
		else
		{
			float diveDownSpeed = 300;
			movementComponent->SetIsAttacking(true);
			return new BeeDiveDownState(diveDownSpeed);
		}
	}

	return nullptr;
}


void BeeDiveDownState::CreatePaths(GameObject* enemy)
{

	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//attack for bees
	//1st part -> 0
	const auto& trc = enemy->GetComponent<TransformComponent>();

	{
		BezierCurve pathCurve{ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y - (screenHeight / 2) },
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 4),  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 8),trc->GetCenterPosition().y + (screenHeight / 8)} };

		path->AddCurve(pathCurve, 15);
		path->Sample(m_Path, 0);
	}

	glm::vec2 playerPos{};
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Coop)
	{
		int playerIndex = rand() % 2;
		if (scene->GetPlayer(playerIndex)->GetComponent<PlayerHealthComponent>()->IsAlive())
		{
			playerPos = scene->GetPlayer(playerIndex)->GetComponent<TransformComponent>()->GetCenterPosition();
		}
		else playerPos = scene->GetPlayer(!playerIndex)->GetComponent<TransformComponent>()->GetCenterPosition();
	}
	else playerPos = scene->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

	//attack point
	m_Path.push_back(playerPos);

	//maneuver back to formation
	//2nd part -> 1
	//if (playerPos.x <= screenWidth / 2)//where do we do return maneuver... Can be randomized. Not neccessary
	{
		BezierCurve pathCurve{ playerPos,
			glm::vec2{playerPos.x, playerPos.y + (screenHeight / 3) },
			glm::vec2{playerPos.x - (screenWidth / 4),  playerPos.y + (screenHeight / 4)},
			enemy->GetComponent<BeeBehaviorComponent>()->GetPosInFormation() };
		
		path->AddCurve(pathCurve, 15);
		path->Sample(m_Path, 1);
	}
}

bool BeeDiveDownState::BeeDiveDown(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{
		if ( m_CurrentWaypoint < int(m_Path.size()) )
		{
			glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

			CheckWaypointDistance(currentPosition);

			if (m_CurrentWaypoint < int(m_Path.size()))
			{
				glm::vec2 translation = m_Direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

				trc->SetPosition(glm::vec3{ currentPosition.x + translation.x, currentPosition.y + translation.y, 0 });
			}
			else
			{
				trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x,m_Path[m_Path.size() - 1].y, 0 });//set position to final point
				m_CurrentWaypoint = -1;
			}
		}

		return false;
	}
	else
	{
		return true;
	}

}
