#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"
#include "Event.h"

class GameObject;
class ScoreEventHandler;

class EnemyManager final : public dae::Singleton<EnemyManager>
{
public:
	~EnemyManager();
	void SpawnEnemies(const std::vector<std::vector<int>>& beeInfo, 
		const std::vector<std::vector<int>>& bfInfo, 
		const std::vector<std::vector<int>>& birdInfo,
		const std::vector<std::shared_ptr<IEventHandler>>& handlers);
	void Update();
	void CleanUp();
	void DeleteEnemy(const std::shared_ptr<GameObject>& gameObject);

	void RandomEnemyShot() const;
	void CalculatePatrolSteps();
	int GetPatrolStep() const;
	void AnEnemyReachedPositionInFormation();
	void SetWaitingForPlayerToRespawn(bool waiting);
	bool GetWaitingForPlayerToRespawn() const { return m_WaitingForPlayerToRespawn; };

private:
	void SendRandomEnemyToAttack();
	void RespawnWaitingHandler();
	void EnemiesHandler();

	float m_SpawnTimer = 0.f;
	float m_SpawnTime = 0.35f;

	float m_RespawnWaitingTimer = 0.f;
	float m_RespawnWaitingTime = 7.f;
	
	//sending to dive down
	float m_DiveDownTimer = 0.f;
	float m_DiveDownTime = 3.0f;

	std::vector<std::vector<int>> m_BeeInfo;
	std::vector<std::vector<int>> m_BFInfo;
	std::vector<std::vector<int>> m_BirdInfo;


	std::vector<std::shared_ptr<GameObject>> m_pEnemies;
	std::vector<std::shared_ptr<GameObject>> m_pEnemyShooters;

	std::shared_ptr<IEventHandler> m_ScoreEventHandler;
	std::shared_ptr<IEventHandler> m_AudioEventHandler;

	int m_NumberOfEnemiesNotInPosition = 0;
	int m_NumberOfEnemiesAlive = 0;

	//patrolling before formation
	float m_StepTimer = 0.f;
	float m_StepTime = 0.3f;
	int m_StepsNumber = 5;
	int m_CurrentStepNumber = 0;

	int m_CurrentDifficultyLevel = 1;

	int m_PanicEnemiesNumber = 8;

	bool m_MovingLeft = true;
	bool m_PanicMode = false;
	bool m_WaitingForPlayerToRespawn = false;
	bool m_BuildingFormation = true;//if not then it is battle stage
	

	//for observer pattern
	Event m_EventLevelCleared;
};

