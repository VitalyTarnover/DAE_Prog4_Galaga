#include "pch.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "SDL_image.h"
#include <string>

void BaseComponent::SetOwnerObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
}





