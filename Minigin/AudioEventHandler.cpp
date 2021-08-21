#include "MiniginPCH.h"
#include "AudioEventHandler.h"

#include "AudioLocator.h"

void AudioEventHandler::OnEvent(GameObject*, const std::string& argument) const
{
	if (argument == "playerShot" || //rocket
		argument == "enemyShot" || //rocket
		argument == "playerDeath" || //collision
		argument == "gameEnd") //scene loader
	{
		PlaySound(argument);
	}
	else if (argument == "BeeKilled" || argument == "AttackingBeeKilled" || 
		argument == "BFKilled" || argument == "AttackingBFKilled" || 
		argument == "BirdKilled" || argument == "AttackingBirdKilled" )
	{
		PlaySound("enemyDeath");
	}
}

void AudioEventHandler::PlaySound(const std::string& soundName) const
{
	AudioLocator::GetAudioService().QueueSound(soundName, 0.2f);
}
