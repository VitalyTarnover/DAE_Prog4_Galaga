#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"
#include "Event.h"


//states: building formation, battle

class GameObject;
class ScoreEventHandler;

class EnemyManager final : public dae::Singleton<EnemyManager>//, public IEventHandler
{
public:
	~EnemyManager();
	void SpawnEnemies(const std::vector<std::vector<int>>& beeInfo, 
		const std::vector<std::vector<int>>& bfInfo, 
		const std::vector<std::vector<int>>& birdInfo,
		std::vector<std::shared_ptr<IEventHandler>> handlers);
	void Update();
	void CleanUp();
	void DeleteEnemy(const std::shared_ptr<GameObject>& gameObject);
	void SendRandomEnemyToAttack();

	void CalculatePatrolSteps();
	int GetPatrolStep();

	void AnEnemyReachedPositionInFormation();

	void SetWaitingForPlayerToRespawn(bool waiting);

private:
	void ShootersShoot();
	float m_SpawnTimer = 0;
	float m_SpawnTime = 0.5f;

	

	//std::vector<glm::vec2> m_EnemiesPosInFormation;
	
	std::vector<std::vector<int>> m_BeeInfo;
	std::vector<std::vector<int>> m_BFInfo;
	std::vector<std::vector<int>> m_BirdInfo;


	std::vector<std::shared_ptr<GameObject>> m_pEnemies;
	std::vector<std::shared_ptr<GameObject>> m_EnemyShooters;

	std::shared_ptr<IEventHandler> m_ScoreEventHandler;

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
	int m_CurrentDifficultyLevel = 1;
	int m_PanicEnemiesNumber = 8;
	bool m_MovingLeft = true;
	bool m_PanicMode = false;
	bool m_WaitingForPlayerToRespawn = false;
	bool m_BuildingFormation;//if not then it is battle stage
	//sending to dive down
	float m_DiveDownTimer = 0;
	float m_DiveDownTime = 3.0f;

	//for observer pattern
	Event m_EventLevelCleared;


};

