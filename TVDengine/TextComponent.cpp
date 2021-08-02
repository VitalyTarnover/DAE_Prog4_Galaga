#include "pch.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"


TextComponent::TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color, bool isVisible)
	:m_Text{ text }
	, m_pFont{ font }
	, m_Color{ color }
	, m_IsVisible{ isVisible }
{

	m_Text;
	m_pFont;
	m_Color;
	m_IsVisible;

}


void TextComponent::SetIsVisible(bool isVisible, float setVisibleFalseTimer)
{
	m_IsVisible = isVisible;


	if (setVisibleFalseTimer != 0)
	{
		m_TimerSet = true;
		m_SetVisibleFalseTimer = setVisibleFalseTimer;
	}
}

void TextComponent::UpdateText(const std::string& text)
{
	m_Text = text;
}

void TextComponent::Update()
{
	if (m_TimerSet)
	{
		m_SetVisibleFalseTimer -= SystemTime::GetInstance().GetDeltaTime();

		if (m_SetVisibleFalseTimer <= 0.0f)
		{
			m_TimerSet = false;
			m_SetVisibleFalseTimer = 0.0f;
			m_IsVisible = false;
		}
	}
}
