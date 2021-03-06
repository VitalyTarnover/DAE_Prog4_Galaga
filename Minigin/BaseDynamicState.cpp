#include "MiniginPCH.h"
#include "BaseDynamicState.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "EnemyManager.h"


BaseDynamicState::BaseDynamicState()
	:BaseEnemyState()
	, m_CurrentWaypoint{}
	, m_Speed{}
	, m_StepSize{}
	, m_Direction{}
{
}

void BaseDynamicState::FollowPath(GameObject* enemy, TransformComponent* trc) 
{
	glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

	CheckWaypointDistance(currentPosition);

	if (m_CurrentWaypoint < int(m_Path.size()))// double check, must be removed
	{
		glm::vec2 translation = m_Direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

		trc->SetPosition(glm::vec3{ currentPosition.x + translation.x, currentPosition.y + translation.y, 0 });
	}
	else
	{
		trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x,m_Path[m_Path.size() - 1].y, 0 });//set position to final point

		EnemyManager::GetInstance().AnEnemyReachedPositionInFormation();
		enemy->GetComponent<BaseEnemyBehaviorComponent>()->SetIsInFormation(true);

		m_CurrentWaypoint = -1;
	}
}

void BaseDynamicState::CheckWaypointDistance(const glm::vec2& currentPosition)
{
	float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));

	if (m_SqrMagnitude > sqrMagnitude) m_SqrMagnitude = sqrMagnitude;
	else
	{
		glm::vec2 distance = m_Path[m_CurrentWaypoint] - currentPosition;
		m_Direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));
	}

	int aproxReachDistance = 8;
	if (sqrMagnitude < aproxReachDistance) ++m_CurrentWaypoint;
}

