#include "MiniginPCH.h"
#include "BaseEnemyMovementComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteAnimComponent.h"

BaseEnemyMovementComponent::BaseEnemyMovementComponent(float stepTime, int stepsNumber, int stepSize)
	:m_StepTime{ stepTime }
	,m_StepsNumber{ stepsNumber }
	,m_StepSize { stepSize }
{
}

void BaseEnemyMovementComponent::Update()
{
	Move();
}

void BaseEnemyMovementComponent::Move()
{
	//Patrolling in formation
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
}
