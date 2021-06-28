#include "MiniginPCH.h"
#include "FighterShipMovementComponent.h"
#include "GameObject.h"

FighterShipMovementComponent::FighterShipMovementComponent(float speed)
	:m_Speed{speed}
	,m_MovingLeft{false}
	,m_MovingRight{false}
{}

void FighterShipMovementComponent::StartMoving(bool left)
{
	if (left) m_MovingLeft = true; 
	else m_MovingRight = true;

}

void FighterShipMovementComponent::StopMoving(bool left)
{
	if (left) m_MovingLeft = false;
	else m_MovingRight = false;
}

void FighterShipMovementComponent::Move()
{
	if ((m_MovingLeft && m_MovingRight) || (!m_MovingLeft && !m_MovingRight)) return;

	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 oldPosition = trc->GetTransform().GetPosition();

	//clamp is required!
	if (m_MovingLeft)trc->SetPosition(glm::vec3(oldPosition.x - (m_Speed * SystemTime::GetInstance().GetDeltaTime()), oldPosition.y, 0));
	else if(m_MovingRight) trc->SetPosition(glm::vec3(oldPosition.x + (m_Speed * SystemTime::GetInstance().GetDeltaTime()), oldPosition.y, 0));
}



void FighterShipMovementComponent::Update()
{
	Move();
}
