#include "MiniginPCH.h"
#include "BirdFlyInState.h"
#include "BirdInFormationState.h"
#include "BezierPath.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteAnimComponent.h"
#include "EnemyManager.h"
#include "BirdBehaviorComponent.h"

BirdFlyInState::BirdFlyInState(float speed, int stepSize)
	:BaseDynamicState()
{
	m_CurrentWaypoint = 0;
	m_Speed = speed;
	m_StepSize = stepSize;
}

BaseEnemyState* BirdFlyInState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)	CreatePaths(enemy);
	
	SetSpriteState(enemy);
	
	Move(enemy);

	if (m_Switch && m_CurrentWaypoint == -1)
	{
		enemy->GetComponent<BirdBehaviorComponent>()->SetIsAttacking(false);
		return new BirdInFormationState();
	}

	return nullptr;
}

void BirdFlyInState::SetSpriteState(GameObject* enemy)
{
	bool spriteOffset = enemy->GetComponent<BirdBehaviorComponent>()->GetIsHurt();

	if (spriteOffset != m_SpriteOffset)
	{
		enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(2);
		m_SpriteOffset = spriteOffset;
	}
}

void BirdFlyInState::CreatePaths(GameObject* enemy)
{

	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for BFs and birds customly mirrored
	//1st part -> 0
	{
		BezierCurve pathCurve{ glm::vec2{screenWidth + (screenWidth / 4), screenHeight},
			glm::vec2{-screenWidth / 4, -screenHeight / 2 },
			glm::vec2{screenWidth + (screenWidth / 4 * 3), screenHeight / 2},
			glm::vec2{screenWidth / 2,screenHeight / 2} };

		path->AddCurve(pathCurve, 15);
		path->Sample(m_Path, 0);
	}


	//final position
	m_Path.push_back(enemy->GetComponent<BaseEnemyBehaviorComponent>()->GetPosInFormation());

	
}

void BirdFlyInState::Move(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{

		if (m_CurrentWaypoint < int(m_Path.size()))
		{
			FollowPath(enemy, trc);
		}
	}
	else
	{
		int currentStep = EnemyManager::GetInstance().GetPatrolStep();

		trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x - (currentStep * m_StepSize),m_Path[m_Path.size() - 1].y, 0 });//set position to final point

		bool spriteOffset = enemy->GetComponent<BirdBehaviorComponent>()->GetIsHurt();
		enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(currentStep % 2) + (spriteOffset * 2));
	}

}

