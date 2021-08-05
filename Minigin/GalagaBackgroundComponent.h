#pragma once
#include "BaseComponent.h"

class TransformComponent;
class GalagaBackgroundComponent final : public BaseComponent
{
public:
	void Update() override;
private:
	float m_ScrollSpeed = 100.f;

	TransformComponent* m_pBackgroundTransformComponent = nullptr;//TODO: place p where needed!
};

