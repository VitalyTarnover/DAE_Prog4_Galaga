#pragma once
#include "Singleton.h"
#include <SDL.h>


//important collisions: player + enemy (happens only if that enemy is diving down), enemy + rocket moving up, player + rocket moving down

class GameObject;

class CollisionManager final : public dae::Singleton<CollisionManager>
{
public:
	void AddGameObjectForCheck(bool isEnemy, const std::shared_ptr<GameObject>& newGameObject);//if not enemy -> it is rocket
	void DeleteGameObjectForCheck(const std::shared_ptr<GameObject>& gameObject);
	void CleanUp();

	void Update();
private:
	bool CheckIfCollide(const SDL_Rect& rect1, const SDL_Rect& rect2);

	std::vector<std::shared_ptr<GameObject>> m_pEnemiesForCheck;
	std::vector<std::shared_ptr<GameObject>> m_pRocketsForCheck;

	std::shared_ptr<GameObject> m_FS1;//Fighter ship, basically player
	std::shared_ptr<GameObject> m_FS2;//for player 2
};

