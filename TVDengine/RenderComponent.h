#pragma once
#include "BaseComponent.h"

class TextComponent;
class Texture2DComponent;

class RenderComponent final : public BaseComponent
{
public:
	RenderComponent();
	void Update() override;
	void Render() const;
	
	void LoadTexture(const std::string& filePath);

	std::shared_ptr<dae::Texture2D> GetTexture() const { return m_pTexture2D; };


private:
	void RenderText() const;
	void RenderTexture2D() const;
	
	
	std::shared_ptr<dae::Texture2D> m_pTexture2D;

	//extra members for optimization
	std::string m_LastText;

	TextComponent* m_pTextComponent;
	Texture2DComponent* m_pTexture2DComponent;
};

