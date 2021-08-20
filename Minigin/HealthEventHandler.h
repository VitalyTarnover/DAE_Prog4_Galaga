#pragma once
#include "Event.h"

class HealthEventHandler final : public IEventHandler
{
public:
	void OnEvent(GameObject* actor, const std::string& argument) override;
private:
	void UpdateLives(GameObject* actor) const;
};

