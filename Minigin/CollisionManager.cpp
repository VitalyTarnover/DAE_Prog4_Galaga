#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "RocketMovementComponent.h"
#include "RocketManager.h"
#include "EnemyManager.h"
#include "BaseEnemyMovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerHealthComponent.h"
#include "TractorBeamDangerComponent.h"
#include "BirdMovementComponent.h"
#include "ScoreEventHandler.h"

void CollisionManager::AddGameObjectForCheck(const std::shared_ptr<GameObject>& newGameObject)
{
	std::string objectName = newGameObject->GetName();
	if (objectName == "Bee" || objectName == "BF" || objectName == "Bird") m_pEnemiesForCheck.push_back(newGameObject);
	else if (objectName == "Rocket") m_pRocketsForCheck.push_back(newGameObject);
	else if (objectName == "EnemyRocket") m_pEnemyRocketsForCheck.push_back(newGameObject);
	else if (objectName == "TractorBeam") m_pTractorBeamsForCheck.push_back(newGameObject);
}

void CollisionManager::AddPlayerCollision(std::shared_ptr<GameObject> newPlayer)
{
	m_pPlayers.push_back(newPlayer);
}

void CollisionManager::DeleteGameObjectForCheck(const std::shared_ptr<GameObject>& gameObject)//TODO: is it needed?
{

	for (size_t i = 0; i < m_pRocketsForCheck.size(); ++i)
	{
		if (m_pRocketsForCheck[i] == gameObject)
		{
			m_pRocketsForCheck.erase(std::remove(m_pRocketsForCheck.begin(), m_pRocketsForCheck.end(), m_pRocketsForCheck[i]), m_pRocketsForCheck.end());
			return;
		}
			
	}

	for (size_t i = 0; i < m_pEnemiesForCheck.size(); ++i)
	{
		if (m_pEnemiesForCheck[i] == gameObject)
		{
			m_pEnemiesForCheck.erase(std::remove(m_pEnemiesForCheck.begin(), m_pEnemiesForCheck.end(), m_pEnemiesForCheck[i]), m_pEnemiesForCheck.end());
			return;
		}
	}


}

void CollisionManager::CleanUp()
{
	for (size_t i = 0; i < m_pEnemiesForCheck.size(); ++i)
	{
		m_pEnemiesForCheck[i] = nullptr;
	}
	m_pEnemiesForCheck.clear();

	for (size_t i = 0; i < m_pRocketsForCheck.size(); ++i)
	{
		m_pRocketsForCheck[i] = nullptr;
	}
	m_pRocketsForCheck.clear();

	for (auto player : m_pPlayers) player = nullptr;
	m_pPlayers.clear();

	for (size_t i = 0; i < m_pEvents.size(); i++)
	{
		m_pEvents[i]->ResetHandlers();
	}
}

void CollisionManager::Update()
{
	//killing player
	
	for (auto player : m_pPlayers)
	{
		if (player && player->GetComponent<PlayerHealthComponent>()->IsAlive())
		{
			SDL_Rect fs1Rect = player->GetComponent<TransformComponent>()->GetRect();

			for (size_t i = 0; i < m_pEnemiesForCheck.size(); i++)
			{
				if (CheckIfCollide(fs1Rect, m_pEnemiesForCheck[i]->GetComponent<TransformComponent>()->GetRect()))
				{
					m_pEvents[0]->Notify(player.get(), "PlayerKilled");
					EnemyManager::GetInstance().DeleteEnemy(m_pEnemiesForCheck[i]);

					m_pEnemiesForCheck[i]->GetComponent<BaseEnemyMovementComponent>()->Die(player);

					m_pEnemiesForCheck.erase(std::remove(m_pEnemiesForCheck.begin(), m_pEnemiesForCheck.end(), m_pEnemiesForCheck[i]), m_pEnemiesForCheck.end());

					break;//so only one thingy kills player
				}
			}

			for (size_t i = 0; i < m_pEnemyRocketsForCheck.size(); ++i)
			{
				if (!m_pEnemyRocketsForCheck[i]->GetMarkedForDelete())
				{
					if (!m_pEnemyRocketsForCheck[i]->GetComponent<RocketMovementComponent>()->GetMovesUp())//so it goes down and is danger to player
					{
						if (CheckIfCollide(fs1Rect, m_pEnemyRocketsForCheck[i]->GetComponent<TransformComponent>()->GetRect()))
						{
							//player->die
							m_pEvents[0]->Notify(player.get(), "PlayerKilled");
							m_pEnemyRocketsForCheck[i]->SetMarkedForDelete(true);
							m_pEnemyRocketsForCheck[i] = nullptr;
							m_pEnemyRocketsForCheck.erase(std::remove(m_pEnemyRocketsForCheck.begin(), m_pEnemyRocketsForCheck.end(), m_pEnemyRocketsForCheck[i]), m_pEnemyRocketsForCheck.end());

							break;
						}

					}
				}
				else m_pEnemyRocketsForCheck.erase(std::remove(m_pEnemyRocketsForCheck.begin(), m_pEnemyRocketsForCheck.end(), m_pEnemyRocketsForCheck[i]), m_pEnemyRocketsForCheck.end());
			}

			for (size_t i = 0; i < m_pTractorBeamsForCheck.size(); ++i)
			{
				if (!m_pTractorBeamsForCheck[i]->GetMarkedForDelete())
				{

					if (CheckIfCollide(fs1Rect, m_pTractorBeamsForCheck[i]->GetComponent<TransformComponent>()->GetRect()))
					{
						//player->die
						m_pEvents[0]->Notify(player.get(), "PlayerKilled");
						//m_pTractorBeamsForCheck[i]->GetComponent<TractorBeamDangerComponent>()->GetBirdOwner()->
						//	GetComponent<BirdMovementComponent>()->FighterCaptured();

						m_pTractorBeamsForCheck[i]->GetComponent<TractorBeamDangerComponent>()->FighterCaptured();

						break;
					}
				}
				else m_pTractorBeamsForCheck.erase(std::remove(m_pTractorBeamsForCheck.begin(), m_pTractorBeamsForCheck.end(), m_pTractorBeamsForCheck[i]), m_pTractorBeamsForCheck.end());
			}
		}
	}
	

	//killing enemies
	for (size_t i = 0; i < m_pRocketsForCheck.size(); ++i)
	{
		if (!m_pRocketsForCheck[i]->GetMarkedForDelete())//TODO: How come that it isn't nullptr-ed by Scene (delete marked objects), i was trying to check if it is nullptr and it was always something!
		{
			//TODO: for optimization you can check if the enemy is in formation and then if the rocket is higher than the zone where formation doesn't start yet
			RocketMovementComponent* rmc = m_pRocketsForCheck[i]->GetComponent<RocketMovementComponent>();
			if (rmc->GetMovesUp())
			{
				SDL_Rect rocketRect = m_pRocketsForCheck[i]->GetComponent<TransformComponent>()->GetRect();
				for (size_t j = 0; j < m_pEnemiesForCheck.size(); ++j)
				{
					if (CheckIfCollide(rocketRect, m_pEnemiesForCheck[j]->GetComponent<TransformComponent>()->GetRect()))
					{
						BirdMovementComponent* enemyBird = m_pEnemiesForCheck[j]->GetComponent<BirdMovementComponent>();
						
						if (enemyBird) enemyBird->Hurt(rmc->GetOwner());

						if (!enemyBird || (enemyBird && m_pEnemiesForCheck[j]->GetMarkedForDelete()))
						{
							EnemyManager::GetInstance().DeleteEnemy(m_pEnemiesForCheck[j]);
							if (!m_pEnemiesForCheck[j]->GetComponent<BaseEnemyMovementComponent>()->GetIsInFormation())
								EnemyManager::GetInstance().AnEnemyReachedPositionInFormation();

							m_pEnemiesForCheck[j]->GetComponent<BaseEnemyMovementComponent>()->Die(rmc->GetOwner());
							m_pEnemiesForCheck.erase(std::remove(m_pEnemiesForCheck.begin(), m_pEnemiesForCheck.end(), m_pEnemiesForCheck[j]), m_pEnemiesForCheck.end());
							RocketManager::GetInstance().ShotHit();
						}

						m_pRocketsForCheck[i]->SetMarkedForDelete(true);
						m_pRocketsForCheck[i] = nullptr;
						m_pRocketsForCheck.erase(std::remove(m_pRocketsForCheck.begin(), m_pRocketsForCheck.end(), m_pRocketsForCheck[i]), m_pRocketsForCheck.end());

						break;
					}
				}

			}
		}
		else m_pRocketsForCheck.erase(std::remove(m_pRocketsForCheck.begin(), m_pRocketsForCheck.end(), m_pRocketsForCheck[i]), m_pRocketsForCheck.end());
	}

}

void CollisionManager::InitializeEvents(std::vector<std::shared_ptr<IEventHandler>> eventHandlers)
{
	std::shared_ptr<Event> eventPlayerKilled = std::make_shared<Event>();
	
	m_pEvents.push_back(eventPlayerKilled);//0: EventPlayerKilled

	//0 - HealthEH
	//1 - AudioEH
	m_pEvents[0]->AddHandler(eventHandlers[0]); //EventPlayerKilled -> HealthEH
}

bool CollisionManager::CheckIfCollide(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	//if left edge of rect1 is to the left of the right edge of rect2  or  left edge of rect2 is to the left of the right edge of rect1 -> they don't collide
	if ( (rect1.x + rect1.w) < rect2.x || (rect2.x + rect2.w) < rect1.x ) return false;
	//if bottom edge of rect 1 is higher than upper edge of rect 2  or  bottom edge of rect 2 is higher than upper edge of rect 1 -> they don't collide
	if ( (rect1.y + rect1.h) < rect2.y || (rect2.y + rect2.h) < rect1.y ) return false;


	return true;
}
