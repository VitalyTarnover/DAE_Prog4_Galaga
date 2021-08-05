#include "MiniginPCH.h"
#include "TractorBeamComponent.h"
#include "SpriteAnimComponent.h"
#include "GameObject.h"

TractorBeamComponent::TractorBeamComponent()
{
}

void TractorBeamComponent::Update()
{
	if (!m_IsInitialized)
	{
		m_TractorBeamAnimation = m_pGameObject->GetComponent<SpriteAnimComponent>();
		m_IsInitialized = true;
	}

	if (m_TractorBeamAnimation)
	{
		m_TractorBeamAnimation->NextFrame();
	}

}
