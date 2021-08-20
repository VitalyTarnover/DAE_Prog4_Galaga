#pragma once
#include "Event.h"


class AudioEventHandler final : public IEventHandler
{
public:
	void OnEvent(GameObject* actor, const std::string& argument) override;
private:
	void PlaySound(std::string soundName) const;
};

