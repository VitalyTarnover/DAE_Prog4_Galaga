#include "MiniginPCH.h"
#include "GalagaBackgroundComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "SceneManager.h"


void GalagaBackgroundComponent::Update()
{
	if (!m_IsInitialized)
	{
		m_pBackgroundTransformComponent = m_pGameObject->GetComponent<TransformComponent>();
		m_IsInitialized = true;
	}

	glm::vec3 oldPos = m_pBackgroundTransformComponent->GetTransform().GetPosition();

	if (oldPos.y <= dae::SceneManager::GetInstance().GetScreenHeight())
	{
		m_pBackgroundTransformComponent->SetPosition(glm::vec3{ oldPos.x, oldPos.y + (SystemTime::GetInstance().GetDeltaTime() * m_ScrollSpeed), 0 });
	}
	else
	{
		m_pBackgroundTransformComponent->SetPosition(glm::vec3{ oldPos.x,-dae::SceneManager::GetInstance().GetScreenHeight(), 0 });
	}


}
