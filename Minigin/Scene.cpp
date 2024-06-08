#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	object->SetScene(this);
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	object.get()->ClearComponents();
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Reload()
{
	RemoveAll();

}

void Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update(float deltaTime)
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove;

    for (auto& object : m_objects)
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
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

