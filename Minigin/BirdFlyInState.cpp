#include "MiniginPCH.h"
#include "BirdFlyInState.h"
#include "InFormationState.h"
#include "BezierPath.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteAnimComponent.h"
#include "EnemyManager.h"

BirdFlyInState::BirdFlyInState(float speed, int stepSize)
	:BaseEnemyState()
	, m_CurrentWaypoint{ 0 }
	, m_Speed{ speed }
	, m_StepSize{ stepSize }
{
}

BaseEnemyState* BirdFlyInState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	BirdFlyIn(enemy);

	if (m_Switch)
	{
		return new InFormationState();
	}

	return nullptr;
}

void BirdFlyInState::CreatePaths(GameObject* enemy)
{

	BezierPath* path = new BezierPath();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for BFs and birds customly mirrored
	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth + (screenWidth / 4), screenHeight}, glm::vec2{-screenWidth / 4, -screenHeight / 2 }, glm::vec2{screenWidth + (screenWidth / 4 * 3), screenHeight / 2}, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(enemy->GetComponent<BaseEnemyMovementComponent>()->GetPosInFormation());

	delete path;
}

void BirdFlyInState::BirdFlyIn(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{

		if (m_CurrentWaypoint < m_Path.size())
		{
			glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

			//check if we have reached next waypoint 
			float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));

			if (sqrMagnitude < 4) //TODO: 4 can be something else, mby even declared 
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

				EnemyManager::GetInstance().AnEnemyReachedPositionInFormation();
				enemy->GetComponent<BaseEnemyMovementComponent>()->SetIsInFormation(true);
				//m_EnemyReachedPosInFormation.Notify();

				m_CurrentWaypoint = -1;
			}
		}
	}
	else
	{
		int currentStep = EnemyManager::GetInstance().GetPatrolStep();

		trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x - (currentStep * m_StepSize),m_Path[m_Path.size() - 1].y, 0 });//set position to final point

		enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(currentStep % 2));

	}

}
