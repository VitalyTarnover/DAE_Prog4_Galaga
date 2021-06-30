#pragma once
#include "BaseComponent.h"

class TransformComponent final : public BaseComponent 
{
public:
	TransformComponent(const glm::vec3& pos, const glm::vec2& scale = glm::vec2{ 1,1 });

	void SetPosition(const glm::vec3& position);

	void SetScale(const glm::vec2& scale);

	const SDL_Rect& GetRect() { return m_SpriteRect; }

	dae::Transform GetTransform() const { return m_Transform; };
private:
	dae::Transform m_Transform;
	SDL_Rect m_SpriteRect;
};