#include "MiniginPCH.h"
#include "BeeDiveDownState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "BeeMovementComponent.h"
#include "InFormationState.h"
#include "EnemyManager.h"

BeeDiveDownState::BeeDiveDownState(float speed)
	:m_Speed{speed}
	,m_CurrentWaypoint{0}
{
}

BaseEnemyState* BeeDiveDownState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	if (BeeDiveDown(enemy))
	{
		BeeMovementComponent* movementComponent = enemy->GetComponent<BeeMovementComponent>();
		
		movementComponent->SetIsAttacking(false);

		if (!movementComponent->GetIsPanicing())return new InFormationState();
		else
		{
			float diveDownSpeed = 300;
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
	//TODO: mby make it of speccific attacking color
	const auto& trc = enemy->GetComponent<TransformComponent>();

	//no need to mirror
	{
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y - (screenHeight / 2) },
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 4),  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 8),trc->GetCenterPosition().y + (screenHeight / 8)} },
			15);
		path->Sample(&m_Path, 0);
	}

	glm::vec2 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

	//attack point
	m_Path.push_back(playerPos);

	//maneuver back to formation
	//2nd part -> 1
	//if (playerPos.x <= screenWidth / 2)//where do we do return maneuver... Can be randomized. Not neccessary
	{
		path->AddCurve({ playerPos,
			glm::vec2{playerPos.x, playerPos.y + (screenHeight / 3) },
			glm::vec2{playerPos.x - (screenWidth / 4),  playerPos.y + (screenHeight / 4)},
			enemy->GetComponent<BeeMovementComponent>()->GetPosInFormation() },
			15);
		path->Sample(&m_Path, 1);
	}
}

bool BeeDiveDownState::BeeDiveDown(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{

		if (m_CurrentWaypoint < m_Path.size())
		{
			glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

			//check if we have reached next waypoint 
			float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));

			if (sqrMagnitude < 6) //TODO: 4 can be something else, mby even declared 
				++m_CurrentWaypoint;

			if (m_CurrentWaypoint < m_Path.size())// double check, must be removed
			{
				glm::vec2 distance = m_Path[m_CurrentWaypoint] - currentPosition;

				glm::vec2 direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));//normalized distance, because of sqrt might be a good idea to cache it

				glm::vec2 translation = direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

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
		//EnemyManager::
		return true;
	}

}
