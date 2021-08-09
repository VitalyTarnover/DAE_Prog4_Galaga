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

	void QueueSound(const SoundNames& key, float volume = 1.0f) override;

	void QueueSound(const MusicNames& key, float volume = 1.0f) override;


	void AddSoundToLibrary(const SoundNames& soundId, const std::string& path) override;

	void AddSoundToLibrary(const MusicNames& soundId, const std::string& path) override;

	void Update() override;

	void Pause() override;

protected://TODO: should it be since this class is final

	void Play(const SoundNames& key, float volume = 1.0f) override;

	void Play(const MusicNames& key, float volume = 1.0f) override;

	void Start();

	void Stop();


private:
	std::atomic_bool m_Playing;
	std::mutex m_Mutex;
	std::condition_variable m_QueueActive{};
	std::queue<SoundUnit> m_SoundQueue{};

	std::map<MusicNames, Mix_Music*> m_MusicLibrary;
	std::map<SoundNames, Mix_Chunk*> m_SoundLibrary;
};

