#include "MiniginPCH.h"
#include "BFDiveDownState.h"
#include "InFormationState.h"
#include "BezierPath.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "BFMovementComponent.h"
#include "SceneLoader.h"
#include "PlayerHealthComponent.h"

BFDiveDownState::BFDiveDownState(float speed)
	:BaseDynamicState()
{
	m_Speed = speed;
	m_CurrentWaypoint = 0;
}

BaseEnemyState* BFDiveDownState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	if (BFDiveDown(enemy))
	{
		BFMovementComponent* movementComponent = enemy->GetComponent<BFMovementComponent>();

		movementComponent->SetIsAttacking(false);

		if (!movementComponent->GetIsPanicing())return new InFormationState();
		else
		{
			float diveDownSpeed = 300;
			return new BFDiveDownState(diveDownSpeed);
		}
	}

	return nullptr;
}

void BFDiveDownState::CreatePaths(GameObject* enemy)
{
	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();


	if (!enemy->GetComponent<BFMovementComponent>()->GetIsWithBird())
	{
		//1st part -> 0
		const auto& trc = enemy->GetComponent<TransformComponent>();

		//no need in mirroring
		{
			path->AddCurve({ trc->GetCenterPosition(),
				glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y - (screenHeight / 4) },
				glm::vec2{trc->GetCenterPosition().x + (screenWidth / 4),  trc->GetCenterPosition().y - (screenHeight / 4)},
				glm::vec2{trc->GetCenterPosition().x, trc->GetCenterPosition().y} },
				15);
			path->Sample(&m_Path, 0);
		}

		glm::vec2 playerPos{};
		std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().GetCurrentScene();

		if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::Singleplayer)
		{
			int playerIndex = rand() % 2;
			if (scene->GetPlayer(playerIndex)->GetComponent<PlayerHealthComponent>()->IsAlive())
			{
				playerPos = scene->GetPlayer(playerIndex)->GetComponent<TransformComponent>()->GetCenterPosition();
			}
			else playerPos = scene->GetPlayer(!playerIndex)->GetComponent<TransformComponent>()->GetCenterPosition();
		}
		else playerPos = scene->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

		//wiggly decending to player
		
		glm::vec2 tempTrajectoryPoint = m_Path[m_Path.size() - 1];

		glm::vec2 distance = playerPos - glm::vec2{ tempTrajectoryPoint.x,tempTrajectoryPoint.y };

		int samplesNumber = 20;//for wiggly path
		for (size_t i = 0; i < samplesNumber; i++)
		{
			tempTrajectoryPoint.x += distance.x / samplesNumber + (30 * float(sin(i)));//sinus mult can be different
			tempTrajectoryPoint.y += distance.y / samplesNumber;

			m_Path.push_back(tempTrajectoryPoint);
		}

		//going down, beyond lower screen edge, where we have a teleport trigger to upper part
		m_Path.push_back(glm::vec2{ playerPos.x, screenHeight + 100 });

		//the point before the last one to teleport to upper edge
		m_Path.push_back(glm::vec2{ screenWidth / 2, -100 });

		//back to position in formation
		m_Path.push_back(glm::vec2{ enemy->GetComponent<BFMovementComponent>()->GetPosInFormation() });
	}
	else
	{
		//dive down attack for birds and bird companions

		const auto& trc = enemy->GetComponent<TransformComponent>();

		glm::vec2 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();
		//1st part -> 0
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x,  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y},
			glm::vec2{trc->GetCenterPosition().x, trc->GetCenterPosition().y + (screenHeight / 4) } },
			15);
		path->Sample(&m_Path, 0);

		//2nd part -> 1		
		path->AddCurve({ m_Path[m_Path.size() - 1],
			glm::vec2{m_Path[m_Path.size() - 1].x - (screenWidth / 8), m_Path[m_Path.size() - 1].y},
			glm::vec2{m_Path[m_Path.size() - 1].x, m_Path[m_Path.size() - 1].y - (screenHeight / 4)},
			playerPos },
			15);
		path->Sample(&m_Path, 1);


		//going down, beyond lower screen edge, where we have a teleport trigger to upper part
		m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, screenHeight + 100 });

		//the point before the last one to teleport to upper edge
		m_Path.push_back(glm::vec2{ screenWidth / 2, -100 });

		//back to position in formation
		m_Path.push_back(glm::vec2{ enemy->GetComponent<BaseEnemyMovementComponent>()->GetPosInFormation() });

	}

}

bool BFDiveDownState::BFDiveDown(GameObject* enemy)
{
	if (m_CurrentWaypoint < m_Path.size())
	{
		const auto& trc = enemy->GetComponent<TransformComponent>();

		glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

		//check if we have reached next waypoint 
		float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));

		if (m_SqrMagnitude > sqrMagnitude) m_SqrMagnitude = sqrMagnitude;
		else
		{
			glm::vec2 distance = m_Path[m_CurrentWaypoint] - currentPosition;
			m_Direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));
		}

		int aproxReachDistance = 8;
		if (sqrMagnitude < aproxReachDistance) ++m_CurrentWaypoint;

		if (m_CurrentWaypoint < m_Path.size())//TODO: double check, must be removed
		{
			if (m_CurrentWaypoint != m_Path.size() - 2)
			{
				glm::vec2 translation = m_Direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

				trc->SetPosition(glm::vec3{ currentPosition.x + translation.x, currentPosition.y + translation.y, 0 });
			}
			else trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 2].x,m_Path[m_Path.size() - 2].y, 0 });//teleport to upper screen edge. Only for BFs and birds
		}
		else
		{
			trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x,m_Path[m_Path.size() - 1].y, 0 });//set position to final point
			return true;
		}
	}
	return false;
}
