#include "MiniginPCH.h"
#include "EnemyFlyInMovement.h"
#include "GameObject.h"
#include "TransformComponent.h"

EnemyFlyInMovement::EnemyFlyInMovement()
	:m_Speed{150.f}
{
	CreatePaths();
	//glm::vec2 startPosition{ 100.f,100.f };
	//m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec3{ startPosition.x, startPosition.y, 0 });
}


void EnemyFlyInMovement::Update()
{
	FlyIn();
}

void EnemyFlyInMovement::CreatePaths()
{
	BezierPath* path = new BezierPath();

	path->AddCurve({ glm::vec2{0,0}, glm::vec2{0,500}, glm::vec2{500,0}, glm::vec2{500, 500} }, 20);

	path->Sample(&m_Path);
	delete path;
}

void EnemyFlyInMovement::FlyIn()
{
	if (m_CurrentWaypoint < m_Path.size())
	{
		const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

		//check if we have reached next waypoint 
		float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));
		
		if (sqrMagnitude < 3) //TODO: 3 can be something else, mby even declared 
			++m_CurrentWaypoint;

		if (m_CurrentWaypoint < m_Path.size())// double check, must be removed
		{
			glm::vec2 distance = m_Path[m_CurrentWaypoint] - currentPosition;
			
			glm::vec2 direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));//normalized distance

			glm::vec2 translation = direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

			trc->SetPosition(glm::vec3{ currentPosition.x + translation.x, currentPosition.y + translation.y, 0 });
		}
	}
}
