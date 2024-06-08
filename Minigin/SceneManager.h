#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <stdexcept>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void FixedUpdate();
		void Update(float deltaTime);
		void Render();

		void SetActiveScene(const std::string& name);
		Scene& GetActiveScene() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<dae::Scene>> m_scenes;
		int m_activeSceneIndex{ -1 };
	};
}
