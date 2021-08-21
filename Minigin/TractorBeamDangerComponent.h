#pragma once
#include "TractorBeamComponent.h"

class TractorBeamDangerComponent final : public TractorBeamComponent
{
public:
	TractorBeamDangerComponent(float lifeTime) : TractorBeamComponent(lifeTime) {};
	void FighterCaptured() const;
};

