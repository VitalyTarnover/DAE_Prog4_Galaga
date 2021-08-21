#include "MiniginPCH.h"
#include "BFFlyInState.h"
#include "SceneManager.h"
#include "BFBehaviorComponent.h"
#include "BezierPath.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "EnemyManager.h"
#include "SpriteAnimComponent.h"
#include "InFormationState.h"

BFFlyInState::BFFlyInState(float speed, int stepSize)
	:BaseDynamicState()
{
	m_CurrentWaypoint = 0;
	m_Speed = speed;
	m_StepSize = stepSize;
}

BaseEnemyState* BFFlyInState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0)
	{
		CreatePaths(enemy);
	}

	Move(enemy);

	if (m_Switch)
	{
		enemy->GetComponent<BFBehaviorComponent>()->SetIsAttacking(false);
		return new InFormationState();
	}

	return nullptr;
}

void BFFlyInState::CreatePaths(GameObject* enemy)
{
	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for BFs and birds
	//1st part -> 0
	BezierCurve pathCurve{ glm::vec2{-screenWidth / 4, screenHeight},
		glm::vec2{screenWidth , -screenHeight / 4 },
		glm::vec2{-screenWidth / 4 * 3,screenHeight / 4 },
		enemy->GetComponent<BFBehaviorComponent>()->GetPosInFormation() };

	path->AddCurve(pathCurve, 15);
	path->Sample(m_Path, 0);
	
}

void BFFlyInState::Move(GameObject* enemy)
{
	const auto& trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{
		if (m_CurrentWaypoint < int(m_Path.size()) )
		{
			FollowPath(enemy, trc);
		}
	}
	else
	{
		int currentStep = EnemyManager::GetInstance().GetPatrolStep();

		trc->SetPosition(glm::vec3{ m_Path[m_Path.size() - 1].x - (currentStep * m_StepSize),m_Path[m_Path.size() - 1].y, 0 });//set position to final point

		enemy->GetComponent<SpriteAnimComponent>()->SetCurrentFrame(int(currentStep % 2));

	}

}
