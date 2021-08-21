#pragma once
#include "BaseComponent.h"

class TransformComponent final : public BaseComponent 
{
public:
	TransformComponent(const glm::vec3& pos, float width = 1.f, float height = 1.f, float scaleX = 1.f, float scaleY = 1.f);

	void SetPosition(const glm::vec3& position);

	void SetScale(const glm::vec2& scale);

	const SDL_Rect& GetRect() const { return m_SpriteRect; }

	dae::Transform GetTransform() const { return m_Transform; };

	glm::vec3 GetCenterPosition() const;

	void SetCenterPosition(glm::vec3 position);

private:
	dae::Transform m_Transform;
	SDL_Rect m_SpriteRect;
};