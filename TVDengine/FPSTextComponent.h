#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

class FPSTextComponent final : public BaseComponent
{
public:
	FPSTextComponent(const std::shared_ptr<dae::Font>& font);
	~FPSTextComponent();

	void Update() override;
	//void Render() override;
private:

	std::string m_FPS = "0";
	TextComponent* m_pText = nullptr;
};