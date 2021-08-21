#include "MiniginPCH.h"
#include "ScoreEventHandler.h"

#include "TextComponent.h"
#include "ScoreComponent.h"
#include "SceneManager.h"
#include "Scene.h"


void ScoreEventHandler::OnEvent(GameObject* actor, const std::string& argument) const
{
	if (argument == "BeeKilled") UpdateScore(actor, 50);
	else if (argument == "BFKilled") UpdateScore(actor, 80);
	else if (argument == "BirdKilled") UpdateScore(actor, 150);
	else if (argument == "AttackingBeeKilled") UpdateScore(actor, 100);
	else if (argument == "AttackingBFKilled") UpdateScore(actor, 160);
	else if (argument == "AttackingBirdKilled") UpdateScore(actor, 400);
}

void ScoreEventHandler::UpdateScore(GameObject* actor, int score) const
{
	int playerIndex = 0;
	if (actor->GetName() == "Player1") playerIndex = 0;
	else if (actor->GetName() == "Player2") playerIndex = 1;

	//add points
	ScoreComponent* scoreComponent = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex)->GetComponent<ScoreComponent>();
	
	scoreComponent->IncreaseScore(score);

	//update display
	std::shared_ptr<GameObject> pointsDisplay = nullptr;
	if (playerIndex == 0) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("ScoreDisplay");
	else if (playerIndex == 1) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("ScoreDisplay2");
	
	pointsDisplay->GetComponent<TextComponent>()->UpdateText("Score: " + std::to_string(scoreComponent->GetScore()));
}
