#pragma once
#include "Texture2D.h"
#include "Transform.h"
#include "ResourceManager.h"
#include <glm\vec2.hpp>

#include "Renderer.h"
#include "Font.h"
#include "SystemTime.h"
#include <SDL.h>

#include <SDL_ttf.h>

class GameObject;

class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	virtual void Update() {};
	//virtual void Render() const {};
	virtual void SetOwnerObject(GameObject* pGameObject);
protected:
	GameObject* m_pGameObject = nullptr;
	bool m_IsInitialized = false;
};

















