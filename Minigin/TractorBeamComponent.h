#pragma once
#include "BaseComponent.h"

class SpriteAnimComponent;

class TractorBeamComponent : public BaseComponent
{
public:
	TractorBeamComponent(float lifeTime);
	virtual void Update() override;

	void SetBirdOwner(GameObject* birdOwner) { m_pBirdOwner = birdOwner; };
	GameObject* GetBirdOwner() const { return m_pBirdOwner; };
protected:
	SpriteAnimComponent* m_pTractorBeamAnimation = nullptr;
	GameObject* m_pBirdOwner = nullptr;
	float m_LifeTime;
};

