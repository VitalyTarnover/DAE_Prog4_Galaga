#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec3& pos, float width, float height, float scaleX, float scaleY )
	:m_SpriteRect{ SDL_Rect{(int)pos.x,(int)pos.y,(int)(width * scaleX),(int)(height * scaleY)} }
{
	m_Transform.SetPosition(pos.x, pos.y, pos.z);
	m_Transform.SetScale(scaleX, scaleY);
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
	m_SpriteRect.x = (int)position.x;
	m_SpriteRect.y = (int)position.y;
	m_Transform.SetPosition(position.x, position.y, 0);
}

void TransformComponent::SetScale(const glm::vec2& scale)
{
	m_SpriteRect.w *= (int)scale.x;
	m_SpriteRect.h *= (int)scale.y;
	m_Transform.SetScale(float(m_SpriteRect.w), float(m_SpriteRect.h));
}


glm::vec3 TransformComponent::GetCenterPosition() const//TODO: check if y needs subtraction!
{
	return glm::vec3(m_Transform.GetPosition().x + m_SpriteRect.w/2, m_Transform.GetPosition().y - m_SpriteRect.h / 2, 0);
}

void TransformComponent::SetCenterPosition(glm::vec3 position)
{
	m_SpriteRect.x = (int)position.x - m_SpriteRect.w / 2;
	m_SpriteRect.y = (int)position.y + m_SpriteRect.h / 2;
	m_Transform.SetPosition(float(m_SpriteRect.x), float(m_SpriteRect.y), 0);
}
