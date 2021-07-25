#include "MiniginPCH.h"
#include "ExplosionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"	
#include "Texture2DComponent.h"
#include "SpriteAnimComponent.h"

void ExplosionManager::MakeExplosion(glm::vec3 position)
{

	auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

	auto explosion = std::make_shared<GameObject>("Rocket");
	explosion->AddComponent(new TransformComponent(glm::vec3(position.x, position.y, 0)));
	explosion->AddComponent(new Texture2DComponent("Explosion.png", scene->GetSceneScale()));
	explosion->AddComponent(new SpriteAnimComponent(4));//TODO: mby make self-destruction component?
	scene->Add(explosion);

	float explosionTime = m_ExplosionTime;

	m_ExplosionsAndTimers.push_back(std::make_pair(explosion, explosionTime));
}

void ExplosionManager::Update()
{
	ExplosionTimersHandler();
}

void ExplosionManager::ExplosionTimersHandler()
{
	for (size_t i = 0; i < m_ExplosionsAndTimers.size(); i++)
	{
		m_ExplosionsAndTimers[i].second -= SystemTime::GetInstance().GetDeltaTime();
				

		m_ExplosionsAndTimers[i].first->GetComponent<SpriteAnimComponent>()->NextFrame();

		if (m_ExplosionsAndTimers[i].second <= 0.f)
		{
			m_ExplosionsAndTimers[i].first->SetMarkedForDelete(true);
			m_ExplosionsAndTimers.erase(std::remove(m_ExplosionsAndTimers.begin(), m_ExplosionsAndTimers.end(), m_ExplosionsAndTimers[i]), m_ExplosionsAndTimers.end());
		}

	}
}
