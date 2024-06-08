#pragma once
#include "SceneManager.h"
#include <map>
#include "GameObject.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> pObject);
		void Remove(std::shared_ptr<GameObject> pObject);
		void RemoveAll();
		void Reload();

		void FixedUpdate();
		void Update(float deltaTime);
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::vector<std::shared_ptr<GameObject>>& GetObjects() const
		{
			return m_pObjects;
		}

		const std::string& GetName() const { return m_Name; }
	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_pObjects{};
		std::vector<std::shared_ptr<GameObject>> m_pInitialObjects{};

		static unsigned int m_IdCounter; 
	};

}
