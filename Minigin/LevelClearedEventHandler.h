#pragma once
#include "Event.h"

class LevelClearedEventHandler : public IEventHandler
{
public:
	void OnEvent(GameObject* actor, const std::string& argument) const override;
private:
	void UpdateHUD() const;
};

