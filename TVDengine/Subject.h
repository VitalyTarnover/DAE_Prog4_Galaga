#pragma once
#include "GameObject.h"
#include "Observer.h"

class Subject
{
public:
	Subject();
	~Subject();

	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void Notify(const GameObject* actor, OldEvent event);

private:
	std::vector<Observer*> m_pObservers;
};
