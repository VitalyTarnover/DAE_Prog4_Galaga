#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec3& pos, const glm::vec2& scale)
	:m_SpriteRect{ SDL_Rect{(int)pos.x,(int)pos.y,(int)scale.x,(int)scale.y} }
{
	m_Transform.SetPosition(pos.x, pos.y, pos.z);
	m_Transform.SetScale(scale.x, scale.y);
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
	m_SpriteRect.x = (int)position.x;
	m_SpriteRect.y = (int)position.y;
	m_Transform.SetPosition(position.x, position.y, 0);
}

void TransformComponent::SetScale(const glm::vec2& scale)
{
	m_SpriteRect.w = (int)scale.x;
	m_SpriteRect.h = (int)scale.y;
	m_Transform.SetScale(scale.x, scale.y);
}