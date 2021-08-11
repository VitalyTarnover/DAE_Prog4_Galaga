#include "MiniginPCH.h"
#include "ScoreEventHandler.h"

#include "ScoreComponent.h"
#include "SceneManager.h"
#include "Scene.h"


void ScoreEventHandler::OnEvent(GameObject* actor, const std::string& argument)
{
	actor;
	if (argument == "BeeKilled")
	{

	}
	else if (argument == "BFKilled")
	{

	}
	else if (argument == "BirdKilled")
	{

	}
	else if (argument == "AttackingBeeKilled")
	{

	}
	else if (argument == "AttackingBFKilled")
	{

	}
	else if (argument == "AttackingBirdKilled")
	{

	}
}

void ScoreEventHandler::UpdateScore(GameObject* actor, int score) const
{
	int playerIndex = 0;
	if (actor->GetName() == "Player1") playerIndex = 0;
	else if (actor->GetName() == "Player2") playerIndex = 1;

	//add points
	ScoreComponent* scoreComponent = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex)->GetComponent<ScoreComponent>();
	
	scoreComponent->IncreaseScore(score);

	//update dispaly
	std::shared_ptr<dae::SceneObject> pointsDisplay = nullptr;
	if (playerIndex == 0) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("ScoreDisplay");
	else if (playerIndex == 1) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("ScoreDisplay2");
	
	//pointsDisplay->Get

}
