#pragma once
#include "AudioService.h"
#include "AudioNullService.h"

class AudioLocator final
{
public:
	static void SetAudioService(AudioService* ss);

	static AudioService& GetAudioService();

	~AudioLocator();

	static void Reset();

private:
	static AudioService* m_Service;
	static AudioNullService m_NullService;

};

