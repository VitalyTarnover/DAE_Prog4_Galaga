#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"
#include "Event.h"

//states: building formation, battle

class GameObject;

class EnemyManager final : public dae::Singleton<EnemyManager>//, public IEventHandler
{
public:
	~EnemyManager();
	void SpawnEnemies(const std::vector<glm::vec2>& posInFormation);
	void Update();
	void ResetEnemies();
	void DeleteEnemy(const std::shared_ptr<GameObject>& gameObject);
	void SendRandomEnemyToAttack();

	void CalculatePatrolSteps();
	int GetPatrolStep();

	void AnEnemyReachedPositionInFormation();

	void SetWaitingForPlayerToRespawn(bool waiting);
private:
	void ShootersShoot();
	bool m_WaitingForPlayerToRespawn = false;
	float m_SpawnTimer = 0;
	float m_SpawnTime = 0.5f;

	bool m_BuildingFormation;//if not then it is battle stage

	std::vector<glm::vec2> m_BeesPosInFormation;

	std::vector<std::shared_ptr<GameObject>> m_Enemies;
	std::vector<std::shared_ptr<GameObject>> m_EnemyShooters;

	int m_NumberOfEnemiesNotInPosition;
	int m_NumberOfEnemiesAlive;

	int m_ShootingTimeMax = 200;//times 100 to get good random float
	int m_ShootingTimeMin = 75;//times 100 to get good random float
	float m_ShootingTimer = 1.f;

	//patrolling before formation
	float m_StepTimer = 0;
	float m_StepTime = 0.3f;
	int m_StepsNumber = 5;
	int m_CurrentStepNumber = 0;
	bool m_MovingLeft = true;

	//sending to dive down
	float m_DiveDownTimer = 0;
	float m_DiveDownTime = 2.0f;

	//for observer pattern
	//void OnEvent();


};

