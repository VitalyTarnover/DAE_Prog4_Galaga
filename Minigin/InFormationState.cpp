#include "MiniginPCH.h"
#include "InFormationState.h"

#include "BeeDiveDownState.h"
#include "BFDiveDownState.h"
#include "BirdDiveDownState.h"

#include "BaseEnemyBehaviorComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "SceneManager.h"
#include "EnemyManager.h"
#include "SpriteAnimComponent.h"

InFormationState::InFormationState()
{

}

BaseEnemyState* InFormationState::Update(GameObject* enemy)
{
	InitializeStepSize(enemy);
	Patrol(enemy);
	SetSpriteState(enemy);
	
	
	if (m_Switch)
	{
		std::string enemyType = enemy->GetName();
		
		enemy->GetComponent<BaseEnemyBehaviorComponent>()->SetIsAttacking(true);

		float diveDownSpeed = 300;
		if(enemyType == "Bee") return new BeeDiveDownState(diveDownSpeed);
		else if(enemyType == "BF") return new BFDiveDownState(diveDownSpeed);
		//else if (enemyType == "Bird")return new BirdDiveDownState(diveDownSpeed);

	}

	return nullptr;
}


void InFormationState::SetSpriteState(GameObject* enemy)
{
	int currentStep = EnemyManager::GetInstance().GetPatrolStep();
	enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(currentStep % 2));
}

void InFormationState::Patrol(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();
	
	int currentStep = EnemyManager::GetInstance().GetPatrolStep();

	glm::vec2 posInFormation = enemy->GetComponent<BaseEnemyBehaviorComponent>()->GetPosInFormation();

	if (trc->GetCenterPosition().x > dae::SceneManager::GetInstance().GetScreenWidth() / 2)
		trc->SetCenterPosition(glm::vec3{ posInFormation.x + m_StepSize * currentStep, trc->GetCenterPosition().y, trc->GetCenterPosition().z });
	else 
		trc->SetCenterPosition(glm::vec3{ posInFormation.x - m_StepSize * currentStep, trc->GetCenterPosition().y, trc->GetCenterPosition().z });
}

void InFormationState::InitializeStepSize(GameObject* enemy)
{
	if (!m_StepSizeInitialized)//TODO: test out, mby we will have to use bool
	{
		const auto& trc = enemy->GetComponent<TransformComponent>();
		int stepDivisor = 40;
		m_StepSize = abs(int(trc->GetCenterPosition().x) - dae::SceneManager::GetInstance().GetScreenWidth() / 2) / stepDivisor;
		m_StepSizeInitialized = true;
	}
}
