#pragma once
#include "Singleton.h"
#include <iostream>
#include <vector>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render() const;

		std::shared_ptr<Scene> GetSceneByName(const std::string& n) const;
		std::shared_ptr<Scene> GetCurrentScene();
		void SetCurrentScene(const std::shared_ptr<Scene>&);

		void SetScreenWidthHeight(int width, int height)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
		}

		int GetScreenWidth() const { return m_ScreenWidth; };
		int GetScreenHeight() const { return m_ScreenHeight; };


	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_pCurrentScene;

		int m_ScreenWidth;
		int m_ScreenHeight;
	};
}
