#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate()
{
	if (m_pActiveSceneIndex != -1)
	{
		m_pScenes[m_pActiveSceneIndex]->FixedUpdate();
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	if (m_pActiveSceneIndex != -1)
	{
		m_pScenes[m_pActiveSceneIndex]->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	if (m_pActiveSceneIndex != -1)
	{
		m_pScenes[m_pActiveSceneIndex]->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes.push_back(scene);

	// If no scene is active, set the first created scene as active.
	if (m_pActiveSceneIndex == -1)
	{
		m_pActiveSceneIndex = static_cast<int>(m_pScenes.size()) - 1;
	}

	return *scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0; i < m_pScenes.size(); ++i)
	{
		if (m_pScenes[i]->GetName() == name)
		{
			if (static_cast<int>(i) != m_pActiveSceneIndex)
			{
				if (m_pActiveSceneIndex != -1)
				{
					m_pScenes[m_pActiveSceneIndex]->RemoveAll();
				}
				m_pActiveSceneIndex = static_cast<int>(i);
			}
			return;
		}
	}
	throw std::runtime_error("Scene not found: " + name);
}

dae::Scene& dae::SceneManager::GetActiveScene() const
{
	if (m_pActiveSceneIndex == -1)
	{
		throw std::runtime_error("No active scene.");
	}
	return *m_pScenes[m_pActiveSceneIndex];
}
