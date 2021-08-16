#include "MiniginPCH.h"
#include "BeeFlyInState.h"
#include "GameObject.h"
#include "BezierPath.h"
#include "SceneManager.h"

#include "BeeMovementComponent.h"
#include "TransformComponent.h"
#include "SpriteAnimComponent.h"

#include "EnemyManager.h"
#include "InFormationState.h"

BeeFlyInState::BeeFlyInState(float speed, int stepSize)
	:BaseDynamicState()
{
	m_CurrentWaypoint = 0;
	m_Speed = speed;
	m_StepSize = stepSize;
}

BaseEnemyState* BeeFlyInState::Update(GameObject* enemy)
{
	if (m_Path.size() == 0) CreatePaths(enemy);

	Move(enemy);

	if (m_Switch)
	{
		enemy->GetComponent<BeeMovementComponent>()->SetIsAttacking(false);
		return new InFormationState();
	}

	return nullptr;
}

void BeeFlyInState::CreatePaths(GameObject* enemy)
{
	std::shared_ptr<BezierPath> path = std::make_shared<BezierPath>();

	int screenWidth = dae::SceneManager::GetInstance().GetScreenWidth();
	int screenHeight = dae::SceneManager::GetInstance().GetScreenHeight();

	//fly in for bees
	//1st part -> 0
	path->AddCurve({ glm::vec2{screenWidth / 2,0}, glm::vec2{0 , screenHeight / 4 * 3}, glm::vec2{screenWidth,screenHeight / 4 * 3}, glm::vec2{screenWidth / 2,screenHeight / 2} }, 15);
	path->Sample(&m_Path, 0);

	//final position
	m_Path.push_back(enemy->GetComponent<BeeMovementComponent>()->GetPosInFormation());

	
}

void BeeFlyInState::Move(GameObject* enemy)
{
	TransformComponent* trc = enemy->GetComponent<TransformComponent>();

	if (m_CurrentWaypoint != -1)// -1 is stand by state, should as well be switch for patroling before formation is built
	{
		if (m_CurrentWaypoint < m_Path.size())
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

