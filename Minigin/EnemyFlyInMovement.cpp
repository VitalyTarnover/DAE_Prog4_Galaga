#include "MiniginPCH.h"
#include "EnemyFlyInMovement.h"
#include "GameObject.h"
#include "BezierPath.h"

#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

EnemyFlyInMovement::EnemyFlyInMovement(const glm::vec2& posInFormation)
	:m_Speed{250.f}
	,m_PosInFormation{ posInFormation }
{
	//CreatePaths();
	
	//glm::vec2 startPosition{ 100.f,100.f };
	//m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec3{ startPosition.x, startPosition.y, 0 });
}


void EnemyFlyInMovement::Update()
{
	if (!m_TestBool)
	{
		CreatePaths();
		m_TestBool = true;
	}
	FlyIn();
}

void EnemyFlyInMovement::CreatePaths()
{
	BezierPath* path = new BezierPath();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	/*
	
	*/

	/*
	//fly in for bees mirrored (P1 and P2 are swapped)
	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth / 2,0},  glm::vec2{screenWidth,screenHeight / 4 * 3}, glm::vec2{0 , screenHeight / 4 * 3} , glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(m_PosInFormation);
	*/

	/*
	//fly in for BFs and birds
	//1st part -> 0
	path->AddCurve({ glm::vec2{-screenWidth / 4, screenHeight}, glm::vec2{screenWidth , -screenHeight / 4 }, glm::vec2{-screenWidth/4 * 3,screenHeight / 4 }, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(m_PosInFormation);
	*/

	/*
	//fly in for BFs and birds customly mirrored
	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth + (screenWidth / 4), screenHeight}, glm::vec2{-screenWidth / 4, -screenHeight / 2 }, glm::vec2{screenWidth + (screenWidth / 4 * 3), screenHeight / 2}, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(m_PosInFormation);
	*/

	/*
	//attack for bees
	//1st part -> 0
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

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
			m_PosInFormation },
			15);
		path->Sample(&m_Path, 1);
	}
	*/


	/*
	//attack for BFs
	//1st part -> 0
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	//no need in mirroring
	{
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y - (screenHeight / 2) },
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 4),  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x, trc->GetCenterPosition().y} },
			15);
		path->Sample(&m_Path, 0);
	}

	//wiggly decending to player

	glm::vec2 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

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
	m_Path.push_back(glm::vec2{ screenWidth/2, - 100 });

	//back to position in formation
	m_Path.push_back(glm::vec2{ m_PosInFormation });
	*/


	//dive down attack for birds

	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	//mirror required
	if (trc->GetCenterPosition().x <= screenWidth / 2)//where do we do first maneuver... For now it makes first virage towards closer screen edge
	{
		//1st part -> 0
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y},
			glm::vec2{trc->GetCenterPosition().x,  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 4),trc->GetCenterPosition().y } },
			15);
		path->Sample(&m_Path, 0);

		//2nd part -> 1		
		path->AddCurve({ m_Path[m_Path.size() - 1],
			glm::vec2{m_Path[m_Path.size() - 1].x - (screenWidth / 8), m_Path[m_Path.size() - 1].y},
			glm::vec2{m_Path[m_Path.size() - 1].x, m_Path[m_Path.size() - 1].y - (screenHeight / 4)},
			glm::vec2{m_Path[m_Path.size() - 1].x + (screenWidth / 4), m_Path[m_Path.size() - 1].y } },
			15);
		path->Sample(&m_Path, 1);
		
	}
	else
	{
		//1st part -> 0
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x + (screenWidth / 8), trc->GetCenterPosition().y},
			glm::vec2{trc->GetCenterPosition().x,  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 4),trc->GetCenterPosition().y } },
			15);
		path->Sample(&m_Path, 0);

		//2nd part -> 1		
		path->AddCurve({ m_Path[m_Path.size() - 1],
			glm::vec2{m_Path[m_Path.size() - 1].x + (screenWidth / 8), m_Path[m_Path.size() - 1].y},
			glm::vec2{m_Path[m_Path.size() - 1].x, m_Path[m_Path.size() - 1].y - (screenHeight / 4)},
			glm::vec2{m_Path[m_Path.size() - 1].x - (screenWidth / 4), m_Path[m_Path.size() - 1].y } },
			15);
		path->Sample(&m_Path, 1);
	}
	
	//glm::vec2 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();
	//m_Path.push_back(glm::vec2{ playerPos });//aiming for player
	
	//going down, beyond lower screen edge, where we have a teleport trigger to upper part
	m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, screenHeight + 100 });

	//the point before the last one to teleport to upper edge
	m_Path.push_back(glm::vec2{ screenWidth / 2, -100 });

	//back to position in formation
	m_Path.push_back(glm::vec2{ m_PosInFormation });

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
		
		if (sqrMagnitude < 6) //TODO: 3 can be something else, mby even declared 
			++m_CurrentWaypoint;

		if (m_CurrentWaypoint < m_Path.size())// double check, must be removed
		{
			if (m_CurrentWaypoint != m_Path.size() - 2) 
			{
				glm::vec2 distance = m_Path[m_CurrentWaypoint] - currentPosition;

				glm::vec2 direction = distance / sqrt((distance.x * distance.x + distance.y * distance.y));//normalized distance, because of sqrt might be a good idea to cache it

				glm::vec2 translation = direction * SystemTime::GetInstance().GetDeltaTime() * m_Speed;

				trc->SetPosition(glm::vec3{ currentPosition.x + translation.x, currentPosition.y + translation.y, 0 });
			}
			else trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 2].x,m_Path[m_Path.size() - 2].y, 0 });//teleport to upper screen edge. Only for BFs and birds
		}
		else trc->SetPosition(glm::vec3{ m_Path[m_Path.size()-1].x,m_Path[m_Path.size() - 1].y, 0 });//set position to final point
	}
}
