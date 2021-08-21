#include "pch.h"
#include "AudioServiceProvider.h"

AudioServiceProvider::AudioServiceProvider()
{
	m_Playing.store(true);
}

AudioServiceProvider::~AudioServiceProvider()
{
	CleanUp();
}

void AudioServiceProvider::QueueSound(const std::string& key, float volume)
{
	SoundUnit sound{ key,volume };
	std::lock_guard<std::mutex> mLock{ m_Mutex };
	m_SoundQueue.push(sound);
	m_QueueActive.notify_one();
}


void AudioServiceProvider::AddSoundToLibrary(const std::string& soundId, const std::string& path, bool isSound)
{
	if (isSound)
	{
		m_SoundLibrary.insert(std::pair<std::string, Mix_Chunk*>(soundId, Mix_LoadWAV(path.c_str())));
	}
	else
	{
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		m_MusicLibrary.insert(std::pair<std::string, Mix_Music*>(soundId, music));
	}
	
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
			Play(ps.id, ps.volume);
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

void AudioServiceProvider::CleanUp() 
{
	m_Playing.store(false);
	m_QueueActive.notify_one();


	for (auto it = m_MusicLibrary.begin(); it != m_MusicLibrary.end(); it++)
	{
		Mix_FreeMusic(it->second);
	}


	for (auto it = m_SoundLibrary.begin(); it != m_SoundLibrary.end(); it++)
	{
		Mix_FreeChunk(it->second);
	}

	//for (auto const& x : m_MusicLibrary) Mix_FreeMusic(x.second);
	//for (auto const& x : m_SoundLibrary) Mix_FreeChunk(x.second);
}

void AudioServiceProvider::Play(const std::string& key, float volume) const
{
	if (m_SoundLibrary.find(key) != m_SoundLibrary.end())
	{
		Mix_Chunk* sound = m_SoundLibrary.at(key);

		Mix_Volume(-1, int(volume * 128.0f));
		Mix_PlayChannel(-1, sound, 0);
	}
	else if (m_MusicLibrary.find(key) != m_MusicLibrary.end())
	{
		Mix_Music* sound = m_MusicLibrary.at(key);

		Mix_VolumeMusic(int(volume * 128.0f));
		Mix_PlayMusic(sound, 1);
	}
}

void AudioServiceProvider::Start() const
{
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void AudioServiceProvider::Stop() const
{
	Mix_Pause(-1);
	Mix_PauseMusic();
}

