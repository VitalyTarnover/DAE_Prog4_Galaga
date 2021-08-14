#include "MiniginPCH.h"
#include "BirdDiveDownState.h"
#include "BirdInFormationState.h"
#include "GameObject.h"
#include "BezierPath.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "BirdMovementComponent.h"
#include "TractorBeamDangerComponent.h"
#include "CollisionManager.h"

#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"
#include "RenderComponent.h"

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

	TractorBeamAttack(enemy);
	SetSpriteState(enemy);


	if (BirdDiveDown(enemy))
	{
		BirdMovementComponent* movementComponent = enemy->GetComponent<BirdMovementComponent>();

		if (!movementComponent->GetIsPanicing())
		{
			movementComponent->SetIsAttacking(false);
			return new BirdInFormationState();
		}
		else
		{
			float diveDownSpeed = 300;
			movementComponent->SetIsAttacking(true);
			return new BirdDiveDownState(diveDownSpeed);
		}
	}

	return nullptr;
}

void BirdDiveDownState::SetSpriteState(GameObject* enemy)
{
	bool spriteOffset = enemy->GetComponent<BirdMovementComponent>()->GetIsHurt();

	if (spriteOffset != m_SpriteOffset)
	{
		enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(2);
		m_SpriteOffset = spriteOffset;
	}
}

void BirdDiveDownState::TractorBeamAttack(GameObject* enemy)
{
	if (m_TractorBeamActivated)
	{
		//waves lifetime: 1st = 0-6, 2nd = 1-5, 3rd = 2-4
		if (m_TractorBeamTimer <= m_TractorBeamTime)
		{
			
			if (m_TractorBeamStage == TractorBeamStage::Stage0)
			{
				auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
				float scale = scene->GetSceneScale();
				glm::vec3 enemyCenterPos = enemy->GetComponent<TransformComponent>()->GetCenterPosition();
				int enemyHeight = enemy->GetComponent<TransformComponent>()->GetRect().h;

				auto tractorBeam = std::make_shared<GameObject>("TractorBeam");
				tractorBeam->AddComponent(new TransformComponent(glm::vec3(0,0,0),46.f,18.f, scale,scale));
				tractorBeam->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(enemyCenterPos.x, enemyCenterPos.y + enemyHeight, enemyCenterPos.z));
				tractorBeam->AddComponent(new Texture2DComponent("TractorBeam1.png", scale));
				tractorBeam->AddComponent(new SpriteAnimComponent(3));
				tractorBeam->AddComponent(new TractorBeamComponent(6));
				tractorBeam->GetComponent<TractorBeamComponent>()->SetBirdOwner(enemy);
				tractorBeam->AddComponent(new RenderComponent());
				scene->Add(tractorBeam);

				m_TractorBeamStage = TractorBeamStage::Stage1;
			}
			else if (m_TractorBeamStage == TractorBeamStage::Stage1 && m_TractorBeamTimer >= 1.f)
			{
				auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
				float scale = scene->GetSceneScale();
				glm::vec3 enemyCenterPos = enemy->GetComponent<TransformComponent>()->GetCenterPosition();
				int enemyHeight = enemy->GetComponent<TransformComponent>()->GetRect().h;
				float firstWaveHeight = 18.f;

				auto tractorBeam = std::make_shared<GameObject>("TractorBeam");
				tractorBeam->AddComponent(new TransformComponent(glm::vec3(0, 0, 0), 46.f, 16.f, scale, scale));
				tractorBeam->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(enemyCenterPos.x, enemyCenterPos.y + enemyHeight + (firstWaveHeight * scale), enemyCenterPos.z));

				tractorBeam->AddComponent(new Texture2DComponent("TractorBeam2.png", scale));
				tractorBeam->AddComponent(new SpriteAnimComponent(3));
				tractorBeam->AddComponent(new TractorBeamComponent(4));
				tractorBeam->GetComponent<TractorBeamComponent>()->SetBirdOwner(enemy);
				tractorBeam->AddComponent(new RenderComponent());
				scene->Add(tractorBeam);

				m_TractorBeamStage = TractorBeamStage::Stage2;
			}
			else if (m_TractorBeamStage == TractorBeamStage::Stage2 && m_TractorBeamTimer >= 2.f)
			{
				auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
				float scale = scene->GetSceneScale();
				glm::vec3 enemyCenterPos = enemy->GetComponent<TransformComponent>()->GetCenterPosition();
				int enemyHeight = enemy->GetComponent<TransformComponent>()->GetRect().h;
				float firstWaveHeight = 18.f;
				float secondWaveHeight = 16.f;


				auto tractorBeam = std::make_shared<GameObject>("TractorBeam");
				tractorBeam->AddComponent(new TransformComponent(glm::vec3(0,0,0), 46, 19, scale, scale));
				tractorBeam->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(enemyCenterPos.x, enemyCenterPos.y + enemyHeight + ((firstWaveHeight + secondWaveHeight) * scale), enemyCenterPos.z));
				tractorBeam->AddComponent(new Texture2DComponent("TractorBeam3.png", scale));
				tractorBeam->AddComponent(new SpriteAnimComponent(3));
				tractorBeam->AddComponent(new TractorBeamDangerComponent(2));
				tractorBeam->GetComponent<TractorBeamDangerComponent>()->SetBirdOwner(enemy);
				tractorBeam->AddComponent(new RenderComponent());
				scene->Add(tractorBeam);
				m_TractorBeamStage = TractorBeamStage::Stage3;
				CollisionManager::GetInstance().AddGameObjectForCheck(tractorBeam);
			}
		}
		else
		{
			m_TractorBeamActivated = false;//TODO: check if needed
		}

	}
	
}

void BirdDiveDownState::CreatePaths(GameObject* enemy)
{
	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();
	
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
			glm::vec2{playerPos.x, playerPos.y } },
			15);
		path->Sample(&m_Path, 0);

	}

	//going down, beyond lower screen edge, where we have a teleport trigger to upper part
	m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, screenHeight + 100 });

	//the point before the last one to teleport to upper edge
	m_Path.push_back(glm::vec2{ m_Path[m_Path.size() - 1].x, -100 });

	//back to position in formation
	m_Path.push_back(glm::vec2{ enemy->GetComponent<BaseEnemyMovementComponent>()->GetPosInFormation() });

	

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
