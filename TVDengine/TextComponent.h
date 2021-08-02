#pragma once
#include "BaseComponent.h"

class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color, bool isVisible = true);

	void SetIsVisible(bool isVisible, float setVisibleFalseTimer = 0);
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
	
	float m_SetVisibleFalseTimer = {};//TODO: mby move timer system to game code. This is a good feature but components should be simple
	//and have specific functions instead of being able to do everything at once. For cool features a combination of componetns should work

	bool m_IsVisible = false;
	bool m_TimerSet = false;
};