#pragma once
#include <vector>
#include <memory>
#include <string>

class GameObject;

class IEventHandler//this is the base class for managers for example,so they implement this method
{
public:
    virtual void OnEvent(GameObject* actor, const std::string& argument) = 0;
};


class Event 
{
    std::vector<std::shared_ptr<IEventHandler>> m_Handlers;
public:
    void AddHandler(std::shared_ptr<IEventHandler> handler);
    void RemoveHandler(std::shared_ptr<IEventHandler> handler);
    void Notify(GameObject* actor, const std::string& argument);

};




