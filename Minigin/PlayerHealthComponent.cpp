#include "MiniginPCH.h"
#include "PlayerHealthComponent.h"
#include "ExplosionManager.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "FighterShipMovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LevelManager.h"
#include "SceneLoader.h"


PlayerHealthComponent::PlayerHealthComponent(int lives)
    :m_Lives{ lives }
    ,m_Dead {false}
    ,m_Lost{false}
    ,m_RespawnTime{7.0f}
{
    m_RespawnTimer = m_RespawnTime;
}

void PlayerHealthComponent::Update()
{
    if (!m_Lost)
    {
        if (m_Dead)
        {
            if (SceneLoader::GetInstance().GetCurrentGameMode() == GameMode::Coop)
            {
                m_RespawnTimer -= SystemTime::GetInstance().GetDeltaTime();
                if (m_RespawnTimer <= 0)
                {
                    Respawn();
                    m_RespawnTimer = m_RespawnTime;
                }
            }
            else Respawn();
        }
    }
    
}

int PlayerHealthComponent::GetLives() const
{
    return m_Lives;
}

void PlayerHealthComponent::SetLives(int newLives)
{
    m_Lives = newLives;
}

void PlayerHealthComponent::Die()
{
    if (!m_Dead)
    {
        //Make boom
        ExplosionManager::GetInstance().MakeExplosion(m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition());
        //Tell enemy manager that we need every one to be in formation again before we respawn 
        EnemyManager::GetInstance().SetWaitingForPlayerToRespawn(true);
        //Make player invisible
        m_pGameObject->GetComponent<Texture2DComponent>()->SetVisible(false);
        //Lock movement
        m_pGameObject->GetComponent<FighterShipMovementComponent>()->SetMovementLocked(true);
        //-1 life
        --m_Lives;
        //dead
        m_Dead = true;

        if (m_Lives <= 0)
        {
            LevelManager::GetInstance().CheckEndGameConditions(m_pGameObject);
            m_Lost = true;
        }
    }
}

void PlayerHealthComponent::Respawn()
{
    if (m_Lives > 0)
    {
        if (SceneLoader::GetInstance().GetCurrentGameMode() != GameMode::Coop && EnemyManager::GetInstance().GetWaitingForPlayerToRespawn()) return;

        //Make player visible
        m_pGameObject->GetComponent<Texture2DComponent>()->SetVisible(true);
        //Reset position
        glm::vec3 playerPos = m_pGameObject->GetComponent<TransformComponent>()->GetCenterPosition();
        m_pGameObject->GetComponent<TransformComponent>()->SetCenterPosition(glm::vec3(dae::SceneManager::GetInstance().GetScreenWidth() / 2, playerPos.y, 0));
        //Unlock movement and set position to the center
        m_pGameObject->GetComponent<FighterShipMovementComponent>()->SetMovementLocked(false);
        //not dead
        m_Dead = false;
    }
}

bool PlayerHealthComponent::IsAlive() const
{
    return !m_Dead;
}
