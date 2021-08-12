#include "MiniginPCH.h"
#include "BirdInFormationState.h"
#include "GameObject.h"
#include "BirdDiveDownState.h"
#include "BirdMovementComponent.h"
#include "TransformComponent.h"

#include "GameObject.h"

#include "SceneManager.h"
#include "EnemyManager.h"
#include "SpriteAnimComponent.h"


BirdInFormationState::BirdInFormationState() : InFormationState()
{
}

BaseEnemyState* BirdInFormationState::Update(GameObject* enemy)
{
	InFormationState::InitializeStepSize(enemy);
	InFormationState::Patrol(enemy);
	SetSpriteState(enemy);

	float diveDownSpeed = 300;
	
	if (m_Switch)
	{
		enemy->GetComponent<BirdMovementComponent>()->SetIsAttacking(true);
		return new BirdDiveDownState(diveDownSpeed);
	}

	return nullptr;
}

void BirdInFormationState::SetSpriteState(GameObject* enemy)
{
	int currentStep = EnemyManager::GetInstance().GetPatrolStep();
	bool spriteOffset = enemy->GetComponent<BirdMovementComponent>()->GetIsHurt();
	enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(currentStep % 2) + (spriteOffset * 2));
}
