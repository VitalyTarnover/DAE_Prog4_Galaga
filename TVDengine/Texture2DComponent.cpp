#include "pch.h"
#include "Texture2DComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

Texture2DComponent::Texture2DComponent(const std::string& filePath, float scale)
	: m_Scale{ scale }
	, m_FilePath{filePath}
	, m_Visible{true}
{
	//dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void Texture2DComponent::Update()
{
	if (!m_IsInitialized)
	{
		SetTexture(m_FilePath);
		m_IsInitialized = true;
	}
}

void Texture2DComponent::SetTexture(const std::string& filePath)
{
	RenderComponent* renderComponent = m_pGameObject->GetComponent<RenderComponent>();
	if (renderComponent)renderComponent->LoadTexture(filePath);
}

//void Texture2DComponent::Render()
//{
//	if (m_Visible)
//	{
//		glm::vec3 pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
//		if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>())
//		{
//			m_IsInitialized = true;
//			//m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
//		}
//
//		if (m_IsInitialized)
//			//m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
//
//		if (m_pGameObject->GetComponent<SpriteAnimComponent>())
//		{
//			SDL_Rect srcRect = m_pGameObject->GetComponent<SpriteAnimComponent>()->GetSrcRect();
//
//			dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D,
//				(int)pos.x,
//				(int)pos.y,
//				srcRect.w * (int)m_Scale,
//				srcRect.h * (int)m_Scale,
//				srcRect.x, srcRect.y,
//				srcRect.w, srcRect.h);
//		}
//		else dae::Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y, m_Scale);
//	}
//}
void Texture2DComponent::SetVisible(bool visible)
{
	m_Visible = visible;
}
