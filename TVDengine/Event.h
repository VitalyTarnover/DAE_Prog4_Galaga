#pragma once
#include <vector>
#include <memory>
#include <string>

class GameObject;

class IEventHandler
{
public:
    virtual void OnEvent(GameObject* actor, const std::string& argument) const = 0;
};

class Event 
{
    std::vector<std::shared_ptr<IEventHandler>> m_pHandlers;
public:
    void AddHandler(const std::shared_ptr<IEventHandler>& handler);
    void RemoveHandler(const std::shared_ptr<IEventHandler>& handler);
    void Notify(GameObject* actor, const std::string& argument) const;
    void ResetHandlers();
};




