#include "MiniginPCH.h"
#include "BirdDiveDownState.h"
#include "InFormationState.h"
#include "GameObject.h"
#include "BezierPath.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "BirdMovementComponent.h"
#include "TractorBeamComponent.h"
#include "CollisionManager.h"

BirdDiveDownState::BirdDiveDownState(float speed)
	:m_Speed{speed}
	,m_CurrentWaypoint{0}
	,m_BombingAttack{true}
{
}

BaseEnemyState* BirdDiveDownState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	if (BirdDiveDown(enemy)) return new InFormationState();//TODO: you can use switch-bool so the enemy manager will not try to force same enemy twice to dive down

	return nullptr;
}

void BirdDiveDownState::ActivateTractorBeam()
{
	//glm::vec3 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();

	auto scene = dae::SceneManager::GetInstance().GetCurrentScene();


	auto tractorBeam = std::make_shared<GameObject>("TractorBeam");
	//tractorBeam->AddComponent(new TransformComponent(glm::vec3(playerPos.x, playerPos.y - verticalOffset, 0)));
	//tractorBeam->AddComponent(new RenderComponent());
	scene->Add(tractorBeam);
	CollisionManager::GetInstance().AddGameObjectForCheck(tractorBeam);

}

void BirdDiveDownState::CreatePaths(GameObject* enemy)
{
	BezierPath* path = new BezierPath();
	
	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	const auto& trc = enemy->GetComponent<TransformComponent>();

	glm::vec2 playerPos = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetCenterPosition();


	if (enemy->GetComponent<BirdMovementComponent>()->GetIsBombing())
	{
		m_BombingAttack = true;

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

	}
	else
	{
		m_BombingAttack = false;

		//1st part -> 0
		path->AddCurve({ trc->GetCenterPosition(),
			glm::vec2{trc->GetCenterPosition().x,  trc->GetCenterPosition().y - (screenHeight / 4)},
			glm::vec2{trc->GetCenterPosition().x - (screenWidth / 8), trc->GetCenterPosition().y},
			glm::vec2{playerPos.x, playerPos.y - (screenHeight / 8) } },
			15);
		path->Sample(&m_Path, 0);

	}

	//going down, beyond lower screen edge, where we have a teleport trigger to upper part
	m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, screenHeight + 100 });

	//the point before the last one to teleport to upper edge
	m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, -100 });

	//back to position in formation
	m_Path.push_back(glm::vec2{ enemy->GetComponent<BaseEnemyMovementComponent>()->GetPosInFormation() });

	delete path;

}

//TODO: mby make extra inheritance-base-classes for dive downs and fly ins since they all share the same mechanism of get
bool BirdDiveDownState::BirdDiveDown(GameObject* enemy)
{
	//here is the place for alternative behavior
	if (m_CurrentWaypoint < m_Path.size())
	{
		if (!m_BombingAttack && m_CurrentWaypoint == m_Path.size() - 4 && m_TractorBeamTimer < m_TractorBeamTime)//moment where to stop for tractor beam
		{
			if (!m_TractorBeamActivated)
			{
				//activate it somehow
				m_TractorBeamActivated = true;
			}

			m_TractorBeamTimer += SystemTime::GetInstance().GetDeltaTime();
			//TODO: also give some sign to something that... tractor beam should be engaged
		}
		else
		{
			const auto& trc = enemy->GetComponent<TransformComponent>();

			glm::vec2 currentPosition = glm::vec2{ trc->GetTransform().GetPosition().x, trc->GetTransform().GetPosition().y };

			//check if we have reached next waypoint 
			float sqrMagnitude = abs((m_Path[m_CurrentWaypoint].x - currentPosition.x) + (m_Path[m_CurrentWaypoint].y - currentPosition.y));

			if (sqrMagnitude < 6) //TODO: 3 can be something else, mby even declared 
				++m_CurrentWaypoint;

			if (m_CurrentWaypoint < m_Path.size())//TODO: double check, must be removed
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
			else
			{
				trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x,m_Path[m_Path.size() - 1].y, 0 });//set position to final point
				return true;
			}
		}
	}
	return false;


}
