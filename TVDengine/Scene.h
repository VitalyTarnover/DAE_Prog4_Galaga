#pragma once
#include "SceneManager.h"
#include "GameObject.h"



namespace dae
{
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const;
		void Update();
		void Render() const;
		
		float GetSceneScale() const { return m_SceneScale; }

		std::shared_ptr<GameObject> GetObjectByName(const std::string& name) const;
		void AddPlayer(const std::shared_ptr<GameObject>& player);
		std::shared_ptr<GameObject> GetPlayer(int index) const;


		void DeleteGameObject(std::shared_ptr<GameObject> objectToDelete);
		void DeletePlayer(int playerIndex);
		
		void DeleteMarkedForDelteGameObjects();
		
		void ClearScene();

	private:
		//explicit means it cannot be used for implicit conversions and copy-initialization.
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_pPlayers{};

		float m_SceneScale = 2.0f;

		static unsigned int m_IdCounter;
	};
}
