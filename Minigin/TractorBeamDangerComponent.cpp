#include "MiniginPCH.h"
#include "TractorBeamDangerComponent.h"
#include "BirdBehaviorComponent.h"
#include "GameObject.h"

void TractorBeamDangerComponent::FighterCaptured() const
{
	m_pBirdOwner->GetComponent<BirdBehaviorComponent>()->FighterCaptured();
}
