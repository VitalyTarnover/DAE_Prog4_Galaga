#pragma once
#include "Event.h"

class ScoreEventHandler final : public IEventHandler
{
public:
	void OnEvent(GameObject* actor, const std::string& argument) override;
private:
	void UpdateScore(GameObject* actor, int score) const;
};

