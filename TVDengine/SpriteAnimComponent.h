#pragma once
#include "BaseComponent.h"

class SpriteAnimComponent final : public BaseComponent
{
public:
	SpriteAnimComponent(int columnsNr);

	SDL_Rect GetSrcRect();

	int GetColumnsNr() const { return m_ColumnsNr; };
	void SetCurrentFrame(int index) { m_CurrentFrame = index; };
private:
	std::shared_ptr<dae::Texture2D> m_pTexture2D{};
	int m_ColumnsNr;
	int m_CurrentFrame;
};