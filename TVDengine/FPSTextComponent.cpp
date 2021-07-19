#include "pch.h"
#include "FPSTextComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <string>

FPSTextComponent::FPSTextComponent(const std::shared_ptr<dae::Font>& font)
	: m_pText{ new TextComponent(m_FPS, font , SDL_Color{ 255,255,0 }) }
{
}

FPSTextComponent::~FPSTextComponent()
{
	//delete m_pText;//deletion is on the m_pGameObject
}

void FPSTextComponent::Update()
{
	if (!m_IsInitialized && m_pGameObject)
	{
		m_pGameObject->AddComponent(m_pText);
		m_IsInitialized = true;
		//m_pText->SetPosition(m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition()); //obsolete
	}

	m_FPS = "FPS: " + std::to_string(int(1.f / SystemTime::GetInstance().GetDeltaTime()));
	m_pText->UpdateText(m_FPS);
};

void FPSTextComponent::Render()
{
	m_pText->Render();
}