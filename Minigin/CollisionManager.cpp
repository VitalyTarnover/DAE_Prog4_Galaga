#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "RocketMovementComponent.h"
#include "RocketManager.h"

void CollisionManager::AddGameObjectForCheck(bool isEnemy, const std::shared_ptr<GameObject>& newGameObject)
{
	if (isEnemy) m_pEnemiesForCheck.push_back(newGameObject);
	else m_pRocketsForCheck.push_back(newGameObject);
}

void CollisionManager::DeleteGameObjectForCheck(const std::shared_ptr<GameObject>& gameObject)
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

	m_FS1 = nullptr;
	m_FS2 = nullptr;//TODO: Check if it actually must be done. For everything. Those are smart pointers after all...

}

void CollisionManager::Update()
{
	//if (m_FS1)
	//{
	//	SDL_Rect fs1Rect = m_FS1->GetComponent<TransformComponent>()->GetRect();
	//
	//	for (size_t i = 0; i < m_pGameObjectsForCheck.size(); i++)
	//	{
	//
	//		if (CheckIfCollide(fs1Rect, m_pGameObjectsForCheck[i]->GetComponent<TransformComponent>()->GetRect()))
	//		{
	//			if (m_pGameObjectsForCheck[i]->GetName() == "TestEnemy")
	//			{
	//
	//			}
	//		}
	//	}
	//}

	for (size_t i = 0; i < m_pRocketsForCheck.size(); ++i)
	{
		if (!m_pRocketsForCheck[i]->GetMarkedForDelete())//TODO: How come that it isn't nullptr-ed by Scene (delete marked objects), i was trying to check if it is nullptr and it was always something!
		{
			//TODO: for optimization you can check if the enemy is in formation and then if the rocket is higher than the zone where formation doesn't start yet
			if (m_pRocketsForCheck[i]->GetComponent<RocketMovementComponent>()->GetMovesUp())
			{
				SDL_Rect rocketRect = m_pRocketsForCheck[i]->GetComponent<TransformComponent>()->GetRect();
				for (size_t j = 0; j < m_pEnemiesForCheck.size(); ++j)
				{
					if (CheckIfCollide(rocketRect, m_pEnemiesForCheck[j]->GetComponent<TransformComponent>()->GetRect()))
					{
						m_pEnemiesForCheck[j]->SetMarkedForDelete(true);
						m_pEnemiesForCheck[j] = nullptr;
						m_pEnemiesForCheck.erase(std::remove(m_pEnemiesForCheck.begin(), m_pEnemiesForCheck.end(), m_pEnemiesForCheck[j]), m_pEnemiesForCheck.end());

						m_pRocketsForCheck[i]->SetMarkedForDelete(true);
						m_pRocketsForCheck[i] = nullptr;
						m_pRocketsForCheck.erase(std::remove(m_pRocketsForCheck.begin(), m_pRocketsForCheck.end(), m_pRocketsForCheck[i]), m_pRocketsForCheck.end());

						RocketManager::GetInstance().ReduceActiveRocketsNumber();

						break;
					}
				}

			}
		}
		else m_pRocketsForCheck.erase(std::remove(m_pRocketsForCheck.begin(), m_pRocketsForCheck.end(), m_pRocketsForCheck[i]), m_pRocketsForCheck.end());
	}

}

bool CollisionManager::CheckIfCollide(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	//if left edge of rect1 is to the left of the right edge of rect2  or  left edge of rect2 is to the left of the right edge of rect1 -> they don't collide
	if ( (rect1.x + rect1.w) < rect2.x || (rect2.x + rect2.w) < rect1.x ) return false;
	//if bottom edge of rect 1 is higher than upper edge of rect 2  or  bottom edge of rect 2 is higher than upper edge of rect 1 -> they don't collide
	if ( (rect1.y + rect1.h) < rect2.y || (rect2.y + rect2.h) < rect1.y ) return false;


	return true;
}