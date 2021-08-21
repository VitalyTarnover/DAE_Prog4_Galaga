#include "pch.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"


TextComponent::TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color)
	:m_Text{ text }
	, m_pFont{ font }
	, m_Color{ color }
{}


void TextComponent::UpdateText(const std::string& text)
{
	m_Text = text;
}

void TextComponent::Update()
{}
