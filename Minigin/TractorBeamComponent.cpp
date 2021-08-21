#include "MiniginPCH.h"
#include "TractorBeamComponent.h"
#include "SpriteAnimComponent.h"
#include "CollisionManager.h"
#include "GameObject.h"

TractorBeamComponent::TractorBeamComponent(float lifeTime)
	:m_LifeTime{lifeTime}
{
}

void TractorBeamComponent::Update()
{
	if (!m_IsInitialized)
	{
		m_pTractorBeamAnimation = m_pGameObject->GetComponent<SpriteAnimComponent>();
		m_IsInitialized = true;
	}

	if (m_pTractorBeamAnimation)
	{
		m_pTractorBeamAnimation->NextFrame();
	}

	if (m_LifeTime >= 0) m_LifeTime -= SystemTime::GetInstance().GetDeltaTime();
	else m_pGameObject->SetMarkedForDelete(true);

	if (m_pBirdOwner->GetMarkedForDelete())m_pGameObject->SetMarkedForDelete(true);
}
