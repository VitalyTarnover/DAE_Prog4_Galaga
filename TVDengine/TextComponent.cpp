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
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_pTexture = std::make_shared<dae::Texture2D>(texture);

	m_IsInitialized = true;

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

void TextComponent::Render()
{
	if (m_pTexture && m_IsVisible && m_pGameObject)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<dae::Texture2D>(texture);
		glm::vec3 pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);//TODO: when everything will be done mby make separate render component
		//instead of all components having render method. Render component will get all necessary data to make a texture and then it will render it in its update.
	}
};