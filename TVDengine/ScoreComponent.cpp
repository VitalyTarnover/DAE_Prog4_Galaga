#include "pch.h"
#include "ScoreComponent.h"
#include "GameObject.h"

ScoreComponent::ScoreComponent(const unsigned int& score)
	: m_Score{ score }
{}


void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify(Event::ScoreSet);
}

void ScoreComponent::IncreaseScore(const int score)
{
	m_Score += score;
	m_pGameObject->Notify(Event(score));
}