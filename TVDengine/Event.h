#pragma once
#include <vector>
#include <string>

//struct EventArgs {};//this is used to pass some extra info when handling events

class IEventHandler//this is the base class for managers for example,so they implement this method
{
public:
    //virtual void OnEvent(const EventArgs& args) = 0;
    virtual void OnEvent(std::string argument) = 0;
};


class Event 
{
    std::vector<IEventHandler*> m_Handlers;
public:
    void AddHandler(IEventHandler* handler);
    void RemoveHandler(IEventHandler* handler);
    //void Notify(const EventArgs& args)
    void Notify(std::string argument);

};




