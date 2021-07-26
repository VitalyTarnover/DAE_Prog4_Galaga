#include "pch.h"
#include "Subject.h"


Subject::Subject()
{}

Subject::~Subject()
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		delete m_pObservers[i];
	}
	m_pObservers.clear();
}

void Subject::AddObserver(Observer* observer)
{
	m_pObservers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		if (m_pObservers[i] == observer)
		{
			delete m_pObservers[i];
			m_pObservers[i] = nullptr;
			m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), m_pObservers[i]), m_pObservers.end());
		}
	}
}

void Subject::Notify(const GameObject* actor, OldEvent event)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		m_pObservers[i]->OnNotify(actor, event);
	}
}