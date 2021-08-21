#pragma once
#include "BaseComponent.h"

class TextComponent final : public BaseComponent
{
public:
	//explicit means it cannot be used for implicit conversions and copy-initialization.
	TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color);

	void UpdateText(const std::string& text);
	void Update() override;

	std::string GetText() { return m_Text; }
	std::shared_ptr<dae::Font> GetFont() { return m_pFont; }
	SDL_Color GetColor() { return m_Color; }

private:
	//m_Position could have been used as local position, it also was here because text should not have been able to move, so instead of getting transform component
	//every frame to get the same position, it would just remember this position.
	std::string m_Text;
	std::shared_ptr<dae::Font> m_pFont;
	SDL_Color m_Color = {};
};