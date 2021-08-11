#pragma once
#include "Singleton.h"
#include "Event.h"
#include <SDL.h>


//important collisions: player + enemy (happens only if that enemy is diving down), enemy + rocket moving up, player + rocket moving down

class GameObject;

class CollisionManager final : public dae::Singleton<CollisionManager>
{
public:
	void AddGameObjectForCheck(const std::shared_ptr<GameObject>& newGameObject);//if not enemy -> it is rocket
	void SetPlayersCollisions();

	void DeleteGameObjectForCheck(const std::shared_ptr<GameObject>& gameObject);
	void CleanUp();

	void Update();
	void InitializeEvents(std::vector<std::shared_ptr<IEventHandler>>eventHandlers);
private:
	bool CheckIfCollide(const SDL_Rect& rect1, const SDL_Rect& rect2);

	std::vector<std::shared_ptr<GameObject>> m_pEnemiesForCheck;
	std::vector<std::shared_ptr<GameObject>> m_pRocketsForCheck;
	std::vector<std::shared_ptr<GameObject>> m_pTractorBeamsForCheck;

	std::vector<std::shared_ptr<Event>> m_pEvents;
	

	std::shared_ptr<GameObject> m_FS1;//Fighter ship, basically player TODO: mby make it a vector
	std::shared_ptr<GameObject> m_FS2;//for player 2
};

