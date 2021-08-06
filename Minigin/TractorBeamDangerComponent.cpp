#include "MiniginPCH.h"
#include "TractorBeamDangerComponent.h"
#include "BirdMovementComponent.h"
#include "GameObject.h"

void TractorBeamDangerComponent::FighterCaptured() const
{
	m_BirdOwner->GetComponent<BirdMovementComponent>()->FighterCaptured();
}
