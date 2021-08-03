#include "MiniginPCH.h"
#include "BaseEnemyMovementComponent.h"
#include "BaseEnemyState.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteAnimComponent.h"
#include "ExplosionManager.h"
#include "RocketManager.h"

BaseEnemyMovementComponent::BaseEnemyMovementComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:m_CurrentState{nullptr}
	, m_BirdCompanionIndex {birdCompanionIndex}
	, m_PosInFormation{posInFormation}
	, m_Speed{speed}
	, m_InFormation{false}
{
}

BaseEnemyMovementComponent::~BaseEnemyMovementComponent()
{
	delete m_CurrentState;
}

glm::vec2 BaseEnemyMovementComponent::GetPosInFormation()
{
	return m_PosInFormation;
}

float BaseEnemyMovementComponent::GetSpeed()
{
	return m_Speed;
}

	
void BaseEnemyMovementComponent::Update()
{
	BaseEnemyState* state = m_CurrentState->Update(m_pGameObject);
	if (state != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = state;
	}
}

void BaseEnemyMovementComponent::Switch()
{
	m_CurrentState->Switch();
}

void BaseEnemyMovementComponent::ShootARocket()
{
	RocketManager::GetInstance().SpawnEnemyRocket(m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition());
}

void BaseEnemyMovementComponent::Die()
{
	m_CurrentState->Die(m_pGameObject);
	//explosion manager makes boom here
	ExplosionManager::GetInstance().MakeExplosion(m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition());
}

bool BaseEnemyMovementComponent::GetIsInFormation() const
{
	return m_InFormation;
}

void BaseEnemyMovementComponent::SetIsInFormation(bool inFormation)
{
	m_InFormation = inFormation;
}


	
