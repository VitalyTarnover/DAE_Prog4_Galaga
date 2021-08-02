#include "pch.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "SpriteAnimComponent.h"
#include "Texture2DComponent.h"

RenderComponent::RenderComponent()
	: m_LastText{}
	, m_pTexture2D{nullptr}
{
}

void RenderComponent::Update()
{
	if (!m_IsInitialized)
	{
		m_pTextComponent = m_pGameObject->GetComponent<TextComponent>();
		m_pTexture2DComponent = m_pGameObject->GetComponent<Texture2DComponent>();

		if (m_pTexture2DComponent)m_pTexture2D = dae::ResourceManager::GetInstance().LoadTexture(m_pTexture2DComponent->GetFilePath());
		m_IsInitialized = true;
	}
}


void RenderComponent::Render() const
{
	if (m_pGameObject)
	{
		RenderText();
		RenderTexture2D();
	}
}

void RenderComponent::LoadTexture(const std::string& filePath)
{
	m_pTexture2D = dae::ResourceManager::GetInstance().LoadTexture(filePath);
}

void RenderComponent::RenderText() const
{
	if (m_pTextComponent)
	{
		std::string text = m_pTextComponent->GetText();

		if (m_LastText == text) return;

		std::shared_ptr<dae::Font> pFont = m_pTextComponent->GetFont();
		SDL_Color color = m_pTextComponent->GetColor();


		const auto surf = TTF_RenderText_Blended(pFont->GetFont(), text.c_str(), color);
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_FreeSurface(surf);
		std::shared_ptr<dae::Texture2D> newTexture = std::make_shared<dae::Texture2D>(texture);
		glm::vec3 pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*newTexture, pos.x, pos.y);
	}

}

void RenderComponent::RenderTexture2D() const
{
	if (m_pTexture2DComponent)
	{
		if (m_pTexture2DComponent->GetVisible())
		{
			glm::vec3 pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			float scale = m_pTexture2DComponent->GetScale();

				if (m_pGameObject->GetComponent<SpriteAnimComponent>())
				{
					SDL_Rect srcRect = m_pGameObject->GetComponent<SpriteAnimComponent>()->GetSrcRect();

					dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D,
						(int)pos.x,
						(int)pos.y,
						srcRect.w * (int)scale,
						srcRect.h * (int)scale,
						srcRect.x, srcRect.y,
						srcRect.w, srcRect.h);
				}
				else dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y, scale);
		}
	}
}
