#include "pch.h"
#include "Event.h"

void Event::AddHandler(const std::shared_ptr<IEventHandler>& handler)
{
	m_pHandlers.push_back(handler);
}


void Event::RemoveHandler(const std::shared_ptr<IEventHandler>& handler)
{
	for (size_t i = 0; i < m_pHandlers.size(); i++)
	{
		if (m_pHandlers[i] == handler)
		{
			m_pHandlers.erase(std::remove(m_pHandlers.begin(), m_pHandlers.end(), handler), m_pHandlers.end());
			return;
		}
	}
}

void Event::Notify(GameObject* actor, const std::string& argument) const
{
	for (size_t i = 0; i < m_pHandlers.size(); i++)
	{
		m_pHandlers[i]->OnEvent(actor, argument);
	}
}

void Event::ResetHandlers()
{
	m_pHandlers.clear();
}
