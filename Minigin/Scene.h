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
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
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
			return m_objects;
		}

		const std::string& GetName() const { return m_name; }

		/*std::vector<std::shared_ptr<GameObject>> FindObjectsWithTag(const std::string& tag) const
		{
			std::vector<std::shared_ptr<GameObject>> foundObjects;
			for (auto& obj : m_objects)
			{
				if (obj->GetTag() == tag)
				{
					foundObjects.push_back(obj);
				}
			}
			return foundObjects;
		}*/
	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		std::vector<std::shared_ptr<GameObject>> m_initialObjects{};

		static unsigned int m_idCounter; 
	};

}
