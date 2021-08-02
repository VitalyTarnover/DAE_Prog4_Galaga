#include "pch.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SystemTime.h"
#include "Subject.h"
#include "RenderComponent.h"

GameObject::GameObject(const std::string& name)
	:SceneObject(name), m_pActorChanged{ new Subject() }
{}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}
}

void GameObject::Update()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update();
	}
}

void GameObject::Render() const
{
	RenderComponent* renderComponent = GetComponent<RenderComponent>();

	if (renderComponent)
	{
		renderComponent->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	pComponent->SetOwnerObject(this);
	m_pComponents.push_back(pComponent);
}

void GameObject::AddWatcher(Observer* pObserver)
{
	m_pActorChanged->AddObserver(pObserver);
}

void GameObject::Notify(OldEvent event)
{
	m_pActorChanged->Notify(this, event);
}