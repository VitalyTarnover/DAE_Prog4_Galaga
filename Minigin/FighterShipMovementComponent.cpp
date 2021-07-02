#include "MiniginPCH.h"
#include "FighterShipMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"

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

	glm::vec3 oldPositionCenter = trc->GetCenterPosition();
	glm::vec3 oldPosition = trc->GetTransform().GetPosition();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();

	if (m_MovingLeft && oldPositionCenter.x > 0)trc->SetCenterPosition(glm::vec3(oldPositionCenter.x - (m_Speed * SystemTime::GetInstance().GetDeltaTime()), oldPositionCenter.y, 0));
	else if (m_MovingLeft && oldPositionCenter.x <= 0) trc->SetCenterPosition(glm::vec3(0, oldPositionCenter.y, 0));

	if (m_MovingRight && oldPositionCenter.x < screenWidth) trc->SetCenterPosition(glm::vec3(oldPositionCenter.x + (m_Speed * SystemTime::GetInstance().GetDeltaTime()), oldPositionCenter.y, 0));
	else if (m_MovingRight && oldPositionCenter.x >= screenWidth) trc->SetCenterPosition(glm::vec3(screenWidth, oldPositionCenter.y, 0));
	
}



void FighterShipMovementComponent::Update()
{
	Move();
}
