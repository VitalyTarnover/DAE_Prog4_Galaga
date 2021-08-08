#include "pch.h"
#include "Event.h"

void Event::AddHandler(IEventHandler* handler)
{
	m_Handlers.push_back(handler);
}

void Event::RemoveHandler(IEventHandler* handler)
{
	m_Handlers.erase(std::remove(m_Handlers.begin(), m_Handlers.end(), handler), m_Handlers.end());//TODO: check if it is done correctly, cuz usually it is for 
	//loop that checks if such thingy actually exists there... which prevents crash i guess;
}

void Event::Notify(std::string argument)
{
	for (size_t i = 0; i < m_Handlers.size(); i++) m_Handlers[i]->OnEvent(argument);
}
