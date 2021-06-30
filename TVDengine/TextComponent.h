#pragma once
#include "BaseComponent.h"

class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color, bool isVisible = true);

	void SetPosition(const glm::vec3& pos);
	void SetIsVisible(bool isVisible, float setVisibleFalseTimer = 0);
	void UpdateText(const std::string& text);
	void Update() override;
	void Render() override;
private:
	std::string m_Text;
	std::shared_ptr<dae::Font> m_pFont;
	std::shared_ptr<dae::Texture2D> m_pTexture;
	glm::vec3 m_Position = {};
	SDL_Color m_Color = {};
	float m_SetVisibleFalseTimer = {};
	bool m_IsVisible = false;
	bool m_SetTimer = false;
};