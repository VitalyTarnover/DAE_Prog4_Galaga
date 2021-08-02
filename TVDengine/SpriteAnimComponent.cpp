#include "pch.h"
#include "SpriteAnimComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

SpriteAnimComponent::SpriteAnimComponent(int columnsNr)
	:m_ColumnsNr{ columnsNr }
	, m_CurrentFrame{ 0 }
{}

SDL_Rect SpriteAnimComponent::GetSrcRect()
{
	if (!m_IsInitialized)
	{
		m_pTexture2D = m_pGameObject->GetComponent<RenderComponent>()->GetTexture();
		m_IsInitialized = true;
	}


	int textureWidth, textureHeight;
	SDL_QueryTexture(m_pTexture2D.get()->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);

	SDL_Rect srcRect;
	srcRect.h = textureHeight;
	srcRect.w = textureWidth / m_ColumnsNr;
	srcRect.y = 0;
	srcRect.x = srcRect.w * m_CurrentFrame;
	return srcRect;
}