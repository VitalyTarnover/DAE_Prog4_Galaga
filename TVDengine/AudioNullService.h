#pragma once
#include "AudioService.h"

class AudioNullService final : public AudioService
{
public:
	AudioNullService() = default;
	~AudioNullService() override = default;

	void QueueSound(const std::string&, float = 1.0f) override {};

	void Play(const std::string&, float = 1.0f) override {};

	void Pause() override {};

	void Update() override {};

	void AddSoundToLibrary(const std::string&, const std::string&, bool) override {};
};

