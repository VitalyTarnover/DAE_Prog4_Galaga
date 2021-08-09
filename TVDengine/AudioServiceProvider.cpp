#include "pch.h"
#include "AudioServiceProvider.h"

AudioServiceProvider::AudioServiceProvider()
{
	m_Playing.store(true);
}

AudioServiceProvider::~AudioServiceProvider()
{
	m_Playing.store(false);
	m_QueueActive.notify_one();

	if (m_MusicLibrary.find(MusicNames::Background01) != m_MusicLibrary.end())
		Mix_FreeMusic(m_MusicLibrary.at(MusicNames::Background01));
	if (m_MusicLibrary.find(MusicNames::Background02) != m_MusicLibrary.end())
		Mix_FreeMusic(m_MusicLibrary.at(MusicNames::Background02));
	if (m_SoundLibrary.find(SoundNames::DiscSFX) != m_SoundLibrary.end())
		Mix_FreeChunk(m_SoundLibrary.at(SoundNames::DiscSFX));
	if (m_SoundLibrary.find(SoundNames::FallSFX) != m_SoundLibrary.end())
		Mix_FreeChunk(m_SoundLibrary.at(SoundNames::FallSFX));
	if (m_SoundLibrary.find(SoundNames::JumpSFX) != m_SoundLibrary.end())
		Mix_FreeChunk(m_SoundLibrary.at(SoundNames::JumpSFX));
	if (m_SoundLibrary.find(SoundNames::LevelCompleteSFX) != m_SoundLibrary.end())
		Mix_FreeChunk(m_SoundLibrary.at(SoundNames::LevelCompleteSFX));
}

void AudioServiceProvider::QueueSound(const SoundNames& key, float volume)
{
	SoundUnit sound{ int(key),volume,false };
	std::lock_guard<std::mutex> mLock{ m_Mutex };
	m_SoundQueue.push(sound);
	m_QueueActive.notify_one();
}

void AudioServiceProvider::QueueSound(const MusicNames& key, float volume)
{
	SoundUnit sound{ int(key),volume,true };
	std::lock_guard<std::mutex> mLock{ m_Mutex };
	m_SoundQueue.push(sound);
	m_QueueActive.notify_one();
}

void AudioServiceProvider::AddSoundToLibrary(const SoundNames& soundId, const std::string& path)
{
	m_SoundLibrary.insert(std::pair<SoundNames, Mix_Chunk*>(soundId, Mix_LoadWAV(path.c_str())));
}

void AudioServiceProvider::AddSoundToLibrary(const MusicNames& soundId, const std::string& path)
{
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	m_MusicLibrary.insert(std::pair<MusicNames, Mix_Music*>(soundId, music));
}

void AudioServiceProvider::Update()
{
	while (m_Playing.load())
	{
		while (!m_SoundQueue.empty())
		{
			std::unique_lock<std::mutex> mLock{ m_Mutex };
			SoundUnit ps = m_SoundQueue.front();
			m_SoundQueue.pop();

			if (ps.music)
				Play(MusicNames(ps.id), ps.volume);
			else
				Play(SoundNames(ps.id), ps.volume);
		}
		std::unique_lock<std::mutex> guard{ m_Mutex };
		m_QueueActive.wait(guard);
	}
}

void AudioServiceProvider::Pause()
{
	if (Mix_PausedMusic())
		Start();
	else
		Stop();
}

void AudioServiceProvider::Play(const SoundNames& key, float volume)
{
	if (m_SoundLibrary.find(key) != m_SoundLibrary.end())
	{
		auto sound = m_SoundLibrary.at(key);

		Mix_Volume(-1, int(volume * 128.0f));
		Mix_PlayChannel(-1, sound, 0);
	}
	else
	{
		//failed
	}
}

void AudioServiceProvider::Play(const MusicNames& key, float volume)
{
	if (m_MusicLibrary.find(key) != m_MusicLibrary.end())
	{
		//Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
		auto sound = m_MusicLibrary.at(key);

		Mix_VolumeMusic(int(volume * 128.0f));
		Mix_PlayMusic(sound, 1);
	}
	else
	{
		//failed
	}
}

void AudioServiceProvider::Start()
{
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void AudioServiceProvider::Stop()
{
	Mix_Pause(-1);
	Mix_PauseMusic();
}

