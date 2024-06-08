#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> pObject)
{
	pObject->SetScene(this);
	m_pObjects.emplace_back(std::move(pObject));
}

void Scene::Remove(std::shared_ptr<GameObject> pObject)
{
	pObject.get()->ClearComponents();
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pObject), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}

void Scene::Reload()
{
	RemoveAll();

}

void Scene::FixedUpdate()
{
	for (auto& object : m_pObjects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update(float deltaTime)
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove;

    for (auto& object : m_pObjects)
    {
        object->Update(deltaTime);
        if (object->GetRemoveObject())
        {
            objectsToRemove.push_back(object);
        }
    }

    for (auto& object : objectsToRemove)
    {
        Remove(object);
    }
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

