#include "MiniginPCH.h"
#include "BaseEnemyMovementComponent.h"
#include "BaseEnemyState.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteAnimComponent.h"

BaseEnemyMovementComponent::BaseEnemyMovementComponent(float speed, glm::vec2 posInFormation)
	:m_CurrentState{nullptr}
	, m_PosInFormation{posInFormation}
	, m_Speed{speed}
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
	m_CurrentState->Update(m_pGameObject);
}

void BaseEnemyMovementComponent::Switch()
{
	m_CurrentState->Switch();
}

void BaseEnemyMovementComponent::Die()
{
	//explosion manager makes boom here
	m_CurrentState->Die(m_pGameObject);
}


	
