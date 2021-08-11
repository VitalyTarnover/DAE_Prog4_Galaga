#include "pch.h"
#include "Event.h"

void Event::AddHandler(std::shared_ptr<IEventHandler> handler)
{
	m_pHandlers.push_back(handler);
}

void Event::RemoveHandler(std::shared_ptr<IEventHandler> handler)
{
	m_pHandlers.erase(std::remove(m_pHandlers.begin(), m_pHandlers.end(), handler), m_pHandlers.end());//TODO: check if it is done correctly, cuz usually it is for 
	//loop that checks if such thingy actually exists there... which prevents crash i guess;
}

void Event::Notify(GameObject* actor, const std::string& argument)
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
