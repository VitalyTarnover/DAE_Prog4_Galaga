#pragma once
#include "BaseComponent.h"

class SpriteAnimComponent;

class TractorBeamComponent : public BaseComponent
{
public:
	TractorBeamComponent(float lifeTime);
	virtual void Update() override;

protected:
	SpriteAnimComponent* m_TractorBeamAnimation = nullptr;
	float m_LifeTime;
};

