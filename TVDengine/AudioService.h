#pragma once
#include <string>

class AudioService
{
public:
	struct SoundUnit
	{
		std::string id;
		float volume;
	};

	virtual ~AudioService() = default;

	AudioService() = default;
	AudioService(const AudioService&) = delete;
	AudioService& operator=(const AudioService&) = delete;
	AudioService(AudioService&&) = delete;
	AudioService& operator= (AudioService&&) = delete;

	virtual void QueueSound(const std::string&, float volume = 1.0f) = 0;

	virtual void AddSoundToLibrary(const std::string&, const std::string&, bool) = 0;

	virtual void Update() = 0;

	virtual void Pause() = 0;

	virtual void CleanUp() = 0;

protected:

	virtual void Play(const std::string&, float volume = 1.0f) = 0;
};

