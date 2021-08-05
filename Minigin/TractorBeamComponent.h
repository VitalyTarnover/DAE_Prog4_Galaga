#pragma once
#include "BaseComponent.h"

class SpriteAnimComponent;

class TractorBeamComponent final : public BaseComponent
{
public:

	TractorBeamComponent();
	void Update() override;

private:
	SpriteAnimComponent* m_TractorBeamAnimation = nullptr;
	
};

