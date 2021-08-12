#include "MiniginPCH.h"
#include "BirdMovementComponent.h"
#include "BirdFlyInState.h"
#include "BirdDiveDownState.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"
#include "RenderComponent.h"


BirdMovementComponent::BirdMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:BaseEnemyMovementComponent(speed, birdCompanionIndex, posInFormation)
{
	m_CurrentState = new BirdFlyInState(m_Speed);
}

BirdMovementComponent::~BirdMovementComponent()
{
	if(m_pCapturedFighter) m_pCapturedFighter->SetMarkedForDelete(true);//TODO: check if needed
}

void BirdMovementComponent::Update()
{
	BaseEnemyMovementComponent::Update();
	HandleCapturedFighter();
}

void BirdMovementComponent::HandleCapturedFighter()
{
	if (m_HasFighterCaptured)
	{
		if (m_pCapturedFighter)
		{
			glm::vec3 birdCenterPos = m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition();
			int birdHeight = m_pGameObject->GetComponent<TransformComponent>()->GetRect().h;
			m_pCapturedFighter->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(birdCenterPos.x, birdCenterPos.y - birdHeight / 2, birdCenterPos.z));

			if (m_CapturedFighterShootingTimer <= m_CapturedFighterShootingTime)
			{
				m_CapturedFighterShootingTimer += SystemTime::GetInstance().GetDeltaTime();
			}
			else
			{
				m_CapturedFighterShootingTimer = 0;
				ShootARocket();
			}
		}
		else
		{
			auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
			float scale = scene->GetSceneScale();
			glm::vec3 birdCenterPos = m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition();
			int birdHeight = m_pGameObject->GetComponent<TransformComponent>()->GetRect().h;

			auto capturedFighter = std::make_shared<GameObject>("CapturedFighter");
			capturedFighter->AddComponent(new TransformComponent(glm::vec3(0, 0, 0), 15, 16, scale, scale));
			capturedFighter->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(birdCenterPos.x, birdCenterPos.y - birdHeight/2, birdCenterPos.z));
			capturedFighter->AddComponent(new Texture2DComponent("FighterShip.png", scale));
			capturedFighter->AddComponent(new SpriteAnimComponent(2));
			capturedFighter->GetComponent<SpriteAnimComponent>()->NextFrame();
			capturedFighter->AddComponent(new RenderComponent());
			scene->Add(capturedFighter);

			m_pCapturedFighter = capturedFighter;
		}
	}
}

void BirdMovementComponent::Hurt(std::shared_ptr<GameObject> killerObject)
{
	if (!m_IsHurt) m_IsHurt = true;
	else
	{
		Die(killerObject);
		if (GetIsAttacking())GetEventEnemyKilledHandler()->Notify(killerObject.get(), "AttackingBirdKilled");
		else GetEventEnemyKilledHandler()->Notify(killerObject.get(), "BirdKilled");
	}

}


