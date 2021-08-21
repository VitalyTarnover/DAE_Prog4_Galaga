#include "MiniginPCH.h"
#include "BaseEnemyBehaviorComponent.h"
#include "BaseEnemyState.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteAnimComponent.h"
#include "ExplosionManager.h"
#include "RocketManager.h"

BaseEnemyBehaviorComponent::BaseEnemyBehaviorComponent(float speed, int birdCompanionIndex, glm::vec2 posInFormation)
	:m_CurrentState{nullptr}
	, m_BirdCompanionIndex {birdCompanionIndex}
	, m_PosInFormation{posInFormation}
	, m_Speed{speed}
	, m_InFormation{false}
{
	m_pEventEnemyKilled = std::make_shared<Event>();
}

BaseEnemyBehaviorComponent::~BaseEnemyBehaviorComponent()
{
	delete m_CurrentState;
}

glm::vec2 BaseEnemyBehaviorComponent::GetPosInFormation() const
{
	return m_PosInFormation;
}

	
void BaseEnemyBehaviorComponent::Update()
{
	BaseEnemyState* state = m_CurrentState->Update(m_pGameObject);
	if (state != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = state;
	}
}

void BaseEnemyBehaviorComponent::Switch()
{
	m_CurrentState->Switch();
}

void BaseEnemyBehaviorComponent::ShootARocket() const
{
	RocketManager::GetInstance().SpawnEnemyRocket(m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition());
}

void BaseEnemyBehaviorComponent::Die(std::shared_ptr<GameObject> killerObject) const
{
	m_CurrentState->Die(m_pGameObject);
	//explosion manager makes boom here
	ExplosionManager::GetInstance().MakeExplosion(m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition());
}

bool BaseEnemyBehaviorComponent::GetIsInFormation() const
{
	return m_InFormation;
}

void BaseEnemyBehaviorComponent::SetIsInFormation(bool inFormation)
{
	m_InFormation = inFormation;
}


	
