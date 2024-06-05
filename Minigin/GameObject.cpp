#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Render() const
{
    if (m_pTexture != nullptr)
    {
        const auto& pos = m_Transform.GetPosition();
        Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
    }

    for (const auto& component : m_Components)
    {
        component->Render();
    }
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
    m_Transform.SetPosition(x, y, 0.0f);
}

glm::vec2 dae::GameObject::GetPosition()
{
    return m_Transform.GetPosition();
}

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
    m_Components.push_back(component);
}

void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> component)
{
    auto it = std::find(m_Components.begin(), m_Components.end(), component);
    if (it != m_Components.end())
    {
        m_Components.erase(it);
    }
}


bool dae::GameObject::HasComponent(const std::shared_ptr<BaseComponent>& component) const
{
    return std::find(m_Components.begin(), m_Components.end(), component) != m_Components.end();
}

void dae::GameObject::Update(float deltaTime)
{
    for (const auto& component : m_Components)
    {
        component->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate() 
{
    for (const auto& component : m_Components)
    {
        component->FixedUpdate();
    }
}