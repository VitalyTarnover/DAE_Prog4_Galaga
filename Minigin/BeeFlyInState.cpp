#include "MiniginPCH.h"
#include "BeeFlyInState.h"
#include "GameObject.h"
#include "BezierPath.h"
#include "SceneManager.h"

#include "BeeMovementComponent.h"
#include "TransformComponent.h"

#include "EnemyManager.h"

BeeFlyInState::BeeFlyInState(float speed)
	:BaseEnemyState()
	,m_CurrentWaypoint{0}
	, m_Speed{speed}
{
}

BaseEnemyState* BeeFlyInState::Update(GameObject* enemy)
{

	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	BeeFlyIn(enemy);

	//if (m_Switch = true)
	//{
	//	return new 
	//}

	return nullptr;
}

void BeeFlyInState::CreatePaths(GameObject* enemy)
{
	BezierPath* path = new BezierPath();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for bees
	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth / 2,0}, glm::vec2{0 , screenHeight / 4 * 3}, glm::vec2{screenWidth,screenHeight / 4 * 3}, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(enemy->GetComponent<BeeMovementComponent>()->GetPosInFormation());

	delete path;
}

void BeeFlyInState::BeeFlyIn(GameObject* enemy)
{
	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{
		if (m_CurrentWaypoint < m_Path.size())
		{
			const auto& trc = enemy->GetComponent<TransformComponent>();

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
				EnemyManager::GetInstance().AnEnemyReachedPositionInFormation();//TODO: when everything works try to make observer for this
				m_CurrentWaypoint = -1;
			}
		}
	}
}
