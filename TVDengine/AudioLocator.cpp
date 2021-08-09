#include "pch.h"
#include "AudioLocator.h"

AudioService* AudioLocator::m_Service{ nullptr };
AudioNullService AudioLocator::m_NullService;

void AudioLocator::SetAudioService(AudioService* ss)
{
	m_Service = ss == nullptr ? &m_NullService : ss;
}

AudioService& AudioLocator::GetAudioService()
{
	return *m_Service;
}

AudioLocator::~AudioLocator()
{
	if (m_Service) delete m_Service;
}

void AudioLocator::Reset()
{
	if (m_Service) delete m_Service;
}

