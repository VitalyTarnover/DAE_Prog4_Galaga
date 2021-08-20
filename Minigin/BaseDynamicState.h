#pragma once
#include "BaseEnemyState.h"

class TransformComponent;
class BaseDynamicState : public BaseEnemyState
{
public:
	BaseDynamicState();
protected:
	virtual void Move(GameObject* ) {};
	virtual void CreatePaths(GameObject* ) {};
	void FollowPath(GameObject* enemy, TransformComponent* trc);
	void CheckWaypointDistance(glm::vec2 currentPosition);

	std::vector<glm::vec2> m_Path;
	int m_CurrentWaypoint;
	float m_Speed;

	int m_StepSize;//for patrolling before formation
	
	glm::vec2 m_Direction;
	float m_SqrMagnitude = FLT_MAX;
};

