#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate()
{
	if (m_activeSceneIndex != -1)
	{
		m_scenes[m_activeSceneIndex]->FixedUpdate();
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	if (m_activeSceneIndex != -1)
	{
		m_scenes[m_activeSceneIndex]->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	if (m_activeSceneIndex != -1)
	{
		m_scenes[m_activeSceneIndex]->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	// If no scene is active, set the first created scene as active.
	if (m_activeSceneIndex == -1)
	{
		m_activeSceneIndex = static_cast<int>(m_scenes.size()) - 1;
	}

	return *scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0; i < m_scenes.size(); ++i)
	{
		if (m_scenes[i]->GetName() == name)
		{
			if (static_cast<int>(i) != m_activeSceneIndex)
			{
				if (m_activeSceneIndex != -1)
				{
					m_scenes[m_activeSceneIndex]->RemoveAll();
				}
				m_activeSceneIndex = static_cast<int>(i);
			}
			return;
		}
	}
	throw std::runtime_error("Scene not found: " + name);
}

dae::Scene& dae::SceneManager::GetActiveScene() const
{
	if (m_activeSceneIndex == -1)
	{
		throw std::runtime_error("No active scene.");
	}
	return *m_scenes[m_activeSceneIndex];
}
