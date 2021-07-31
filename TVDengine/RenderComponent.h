#pragma once
#include "BaseComponent.h"


class RenderComponent final : public BaseComponent
{
public:
	RenderComponent();
	void Update() override;

	std::shared_ptr<dae::Texture2D> GetTexture() const { return m_pTexture; };


private:
	void RenderText();
	void RenderTexture2D();
	std::shared_ptr<dae::Texture2D> m_pTexture;


};

