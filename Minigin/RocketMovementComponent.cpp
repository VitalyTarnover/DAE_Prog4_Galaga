#include "MiniginPCH.h"
#include "RocketMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RocketManager.h"

RocketMovementComponent::RocketMovementComponent(bool movesUp, float speed) //movesUp also means it was shot by a player
	:m_MovesUp{ movesUp }
	,m_Speed {speed}
{}

void RocketMovementComponent::Update()
{
	Move();
	OutsideBordersCheck();
}

void RocketMovementComponent::Move()
{
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 oldPosition = trc->GetTransform().GetPosition();
	
	if(m_MovesUp)trc->SetPosition(glm::vec3(oldPosition.x, oldPosition.y - (m_Speed * SystemTime::GetInstance().GetDeltaTime()), 0));
	else trc->SetPosition(glm::vec3(oldPosition.x, oldPosition.y + (m_Speed * SystemTime::GetInstance().GetDeltaTime()), 0));

}

void RocketMovementComponent::OutsideBordersCheck()
{
	const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 currentPosition = trc->GetTransform().GetPosition();

	if (currentPosition.y < 0 || currentPosition.y > 1280)//TODO: instead of magic numbers - put screen height
	{
		m_pGameObject->SetMarkedForDelete(true);//check if leaks
		if (m_MovesUp) RocketManager::GetInstance().ReduceActiveRocketsNumber();
	}
}
