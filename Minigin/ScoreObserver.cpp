#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Components.h"

void ScoreObserver::OnNotify(const GameObject*, Event)
{
	
	//UpdateScore(actor);
	
}

void ScoreObserver::UpdateScore(const GameObject* actor)
{
	std::shared_ptr<dae::SceneObject> pointsDisplay = nullptr;
	if (actor->GetName() == "Player1") pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay");
	else if (actor->GetName() == "Player2") pointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay2");
	
	auto textComp = std::static_pointer_cast<GameObject>(pointsDisplay).get()->GetComponent<TextComponent>();

	auto scoreComp = actor->GetComponent<ScoreComponent>();

	textComp->UpdateText("Score: " + std::to_string(scoreComp->GetScore()));
}