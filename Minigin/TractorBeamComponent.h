#pragma once
#include "BaseComponent.h"

class SpriteAnimComponent;

class TractorBeamComponent : public BaseComponent
{
public:
	TractorBeamComponent(float lifeTime);
	virtual void Update() override;

	void SetBirdOwner(GameObject* birdOwner) { m_BirdOwner = birdOwner; };
	GameObject* GetBirdOwner() const { return m_BirdOwner; };
protected:
	SpriteAnimComponent* m_TractorBeamAnimation = nullptr;
	GameObject* m_BirdOwner = nullptr;
	float m_LifeTime;
};

