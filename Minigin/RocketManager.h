#pragma once
#include "Singleton.h"

class RocketManager final : public dae::Singleton<RocketManager>
{
public:

	void ReduceActiveRocketsNumber();
	void SpawnRocket(bool movesUp);

private:
	int m_ActiveRocketsNumber;
	int m_AllowedRocketsNumber;
};

