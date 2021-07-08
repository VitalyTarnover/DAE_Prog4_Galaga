#pragma once
#include "BaseComponent.h"
#include "BezierPath.h"

class EnemyFlyInMovement final: public BaseComponent //TODO: make it a child of base movement or something
{
public:
	EnemyFlyInMovement();
	~EnemyFlyInMovement() = default;


	void Update() override;
private:
	std::vector<glm::vec2> m_Path;//TODO: test purpuses. Later you can ad beziers for fly in, dive down etc etc

	float m_Speed;

	int m_CurrentWaypoint;

	void CreatePaths();
	void FlyIn();

	//TODO: mby add start position and do all the magical movement from it
};