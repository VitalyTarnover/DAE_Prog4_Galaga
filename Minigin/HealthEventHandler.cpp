#include "MiniginPCH.h"
#include "HealthEventHandler.h"
#include "PlayerHealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

void HealthEventHandler::OnEvent(GameObject* actor, const std::string& argument)
{
	if (argument == "PlayerKilled") UpdateLives(actor);
}

void HealthEventHandler::UpdateLives(GameObject* actor) const
{
	//kill player
	PlayerHealthComponent* playerHealthComponent = actor->GetComponent<PlayerHealthComponent>();
	playerHealthComponent->Die();

	//update display
	int playerIndex = 0;
	if (actor->GetName() == "Player1") playerIndex = 0;
	else if (actor->GetName() == "Player2") playerIndex = 1;

	
	std::shared_ptr<GameObject> pointsDisplay = nullptr;
	if (playerIndex == 0) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("LivesDisplay");
	else if (playerIndex == 1) pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("LivesDisplay2");

	pointsDisplay->GetComponent<TextComponent>()->UpdateText("Lives: " + std::to_string(playerHealthComponent->GetLives()));

}

