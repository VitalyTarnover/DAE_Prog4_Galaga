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


	//Patrolling in formation
	/*
	if (m_StepTimer > 0) m_StepTimer -= SystemTime::GetInstance().GetDeltaTime();
	else
	{
		const auto& trc = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec3 oldPosition = trc->GetTransform().GetPosition();

		float sceneScale = dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();

		if (m_MovingLeft)trc->SetPosition(glm::vec3(oldPosition.x - (m_StepSize * sceneScale), oldPosition.y, 0));
		else trc->SetPosition(glm::vec3(oldPosition.x + (m_StepSize * sceneScale), oldPosition.y, 0));
		
		++m_CurrentStepNumber;


		if (m_CurrentStepNumber > m_StepsNumber)
		{
			m_CurrentStepNumber = 0;
			m_MovingLeft = !m_MovingLeft;
		}

		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(m_CurrentStepNumber%2));
		
		m_StepTimer = m_StepTime;
	}
	*/
