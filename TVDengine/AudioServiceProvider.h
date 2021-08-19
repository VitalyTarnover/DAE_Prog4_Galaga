#pragma once
#include "AudioService.h"
#include <SDL_mixer.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <map>


class AudioServiceProvider final : public AudioService
{
public:
	AudioServiceProvider();


	~AudioServiceProvider();

	void QueueSound(const std::string& key, float volume = 1.0f) override;


	void AddSoundToLibrary(const std::string& soundId, const std::string& path, bool isSound) override;


	void Update() override;

	void Pause() override;

	void CleanUp() override;

private:
	void Play(const std::string& key, float volume = 1.0f) override;

	void Start();

	void Stop();

	std::atomic_bool m_Playing;
	std::mutex m_Mutex;
	std::condition_variable m_QueueActive{};
	std::queue<SoundUnit> m_SoundQueue{};

	std::map<std::string, Mix_Music*> m_MusicLibrary;
	std::map<std::string, Mix_Chunk*> m_SoundLibrary;
};

