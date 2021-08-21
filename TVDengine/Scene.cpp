#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "TransformComponent.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;


Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_Objects{}
	, m_pPlayers{}
{}

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::DeleteGameObject(const std::shared_ptr<GameObject>& objectToDelete)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] == objectToDelete)
		{
			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), m_Objects[i]), m_Objects.end());
		}
	}
}

void dae::Scene::DeletePlayer(int playerIndex)
{
	int playersSize = int(m_pPlayers.size());
	if (playersSize < playerIndex && playersSize > 0)
	{
		m_pPlayers.erase(std::remove(m_pPlayers.begin(), m_pPlayers.end(), m_pPlayers[playerIndex]), m_pPlayers.end());
	}
}

void dae::Scene::DeleteMarkedForDelteGameObjects()
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->GetMarkedForDelete())
		{
			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), m_Objects[i]), m_Objects.end());
		}
	}

	for (size_t i = 0; i < m_pPlayers.size(); i++)
	{
		if (m_pPlayers[i]->GetMarkedForDelete())
		{
			m_pPlayers.erase(std::remove(m_pPlayers.begin(), m_pPlayers.end(), m_pPlayers[i]), m_pPlayers.end());
		}
	}
		
}

void dae::Scene::ClearScene()
{
	m_Objects.clear();
	m_pPlayers.clear();
}


void Scene::Update()
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]) m_Objects[i]->Update();
	}
	DeleteMarkedForDelteGameObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (object) object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}


std::shared_ptr<GameObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_Objects)
	{
		if (object->GetName().compare(name) == 0)
		{
			return object;
		}
	}
	return nullptr;
}

void Scene::AddPlayer(const std::shared_ptr<GameObject>& player)
{
	m_pPlayers.push_back(player);
}

std::shared_ptr<GameObject> dae::Scene::GetPlayer(int index) const
{
	if (m_pPlayers.size() <= (unsigned)index)
	{
		return nullptr;
	}
	return m_pPlayers[index];
}




