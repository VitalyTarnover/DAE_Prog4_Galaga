#include "pch.h"
#include "Texture2DComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteAnimComponent.h"

Texture2DComponent::Texture2DComponent(const std::string& filename, float scale)
	:m_pTexture2D(dae::ResourceManager::GetInstance().LoadTexture(filename))
	, m_Scale{ scale }
	, m_Visible{true}
{
}

void Texture2DComponent::Render()
{
	if (m_Visible)
	{
		if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>())
		{
			m_IsInitialized = true;
			m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		}

		if (m_IsInitialized)
			m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		if (m_pGameObject->GetComponent<SpriteAnimComponent>())
		{
			SDL_Rect srcRect = m_pGameObject->GetComponent<SpriteAnimComponent>()->GetSrcRect();

			dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D,
				(int)m_Position.x,
				(int)m_Position.y,
				srcRect.w * (int)m_Scale,
				srcRect.h * (int)m_Scale,
				srcRect.x, srcRect.y,
				srcRect.w, srcRect.h);
		}
		else dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D, m_Position.x, m_Position.y, m_Scale);
	}
}
void Texture2DComponent::SetVisible(bool visible)
{
	m_Visible = visible;
}
