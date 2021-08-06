#pragma once
#include "TractorBeamComponent.h"

class TractorBeamDangerComponent final : public TractorBeamComponent
{
public:
	TractorBeamDangerComponent(float lifeTime) : TractorBeamComponent(lifeTime) {};
	void SetBirdOwner(GameObject* birdOwner) { m_BirdOwner = birdOwner; };
	GameObject* GetBirdOwner() { return m_BirdOwner; };

	void FighterCaptured() const;//TODO: check other functions for such const
private:
	GameObject* m_BirdOwner = nullptr;
};

