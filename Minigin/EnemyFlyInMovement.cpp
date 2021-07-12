#include "MiniginPCH.h"
#include "EnemyFlyInMovement.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"

EnemyFlyInMovement::EnemyFlyInMovement(const glm::vec2& posInFormation)
	:m_Speed{200.f}
	,m_PosInFormation{ posInFormation }
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

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth/2,0}, glm::vec2{0 , screenHeight/ 4 * 3}, glm::vec2{screenWidth,screenHeight / 4 * 3}, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path,0);

	//intermediate point
	m_Path.push_back(m_PosInFormation);

	//2nd part -> 1
	//path->AddCurve({ glm::vec2{500, 500}, glm::vec2{700,300}, glm::vec2{200,300}, glm::vec2{500, 400} }, 15);
	//path->Sample(&m_Path, 1);
	
	

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
