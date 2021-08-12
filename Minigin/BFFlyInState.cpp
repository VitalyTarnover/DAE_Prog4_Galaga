#include "MiniginPCH.h"
#include "BFFlyInState.h"
#include "SceneManager.h"
#include "BFMovementComponent.h"
#include "BezierPath.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "EnemyManager.h"
#include "SpriteAnimComponent.h"
#include "InFormationState.h"

BFFlyInState::BFFlyInState(float speed, int stepSize)
	:BaseEnemyState()
	, m_CurrentWaypoint{ 0 }
	, m_Speed{ speed }
	, m_StepSize{ stepSize }
{
}

BaseEnemyState* BFFlyInState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	BFFlyIn(enemy);

	if (m_Switch)
	{
		enemy->GetComponent<BFMovementComponent>()->SetIsAttacking(false);
		return new InFormationState();
	}

	return nullptr;
}

void BFFlyInState::CreatePaths(GameObject* enemy)
{
	BezierPath* path = new BezierPath();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for BFs and birds
	//1st part -> 0
	path->AddCurve({ glm::vec2{-screenWidth / 4, screenHeight}, glm::vec2{screenWidth , -screenHeight / 4 }, glm::vec2{-screenWidth / 4 * 3,screenHeight / 4 }, enemy->GetComponent<BFMovementComponent>()->GetPosInFormation() }, 15);
	path->Sample(&m_Path, 0);

	delete path;
}

void BFFlyInState::BFFlyIn(GameObject* enemy)
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
