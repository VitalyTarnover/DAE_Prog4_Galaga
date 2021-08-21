#pragma once
#include "Event.h"


class AudioEventHandler final : public IEventHandler
{
public:
	void OnEvent(GameObject* actor, const std::string& argument) const override;
private:
	void PlaySound(const std::string& soundName) const;
};

