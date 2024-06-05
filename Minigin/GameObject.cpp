#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Render() const
{
    for (const auto& component : m_Components)
    {
        component->Render();
    }
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

void dae::GameObject::SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
{
    auto transform = GetComponent<TransformComponent>();
    if (transform == nullptr)
    {
        return;
    }

    if (IsChild(parent) || parent.get() == this || m_pParent.lock().get() == parent.get())
    {
        return;
    }

    if (m_pParent.lock() == nullptr)
    {
        transform->SetLocalPosition(transform->GetWorldPosition());
    }
    else
    {
        auto transformParent = parent == nullptr ? nullptr : parent->GetComponent<TransformComponent>();
        if (keepWorldPosition && transformParent != nullptr)
        {
            transform->SetLocalPosition(transform->GetLocalPosition() - transformParent->GetWorldPosition());
        }
        if (transform)
        {
            transform->SetPositionDirty();
            for (auto& child : m_Children)
            {
                if (auto childTransform = child->GetComponent<TransformComponent>())
                {
                    childTransform->SetPositionDirty();
                }
            }
        }
    }

    if (m_pParent.lock())
    {
        m_pParent.lock()->RemoveChild(shared_from_this());
    }

    m_pParent = parent;

    if (parent != nullptr)
    {
        parent->AddChild(shared_from_this());
    }

   
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
    auto foundObject = std::find(m_Children.begin(), m_Children.end(), child);
    if (foundObject != m_Children.end())
    {
        m_Children.erase(foundObject);
    }
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    m_Children.emplace_back(child);
}

bool dae::GameObject::IsChild(const std::shared_ptr<GameObject>& parent) const
{
    if (!parent)
    {
        return false;
    }

    for (const auto& child : m_Children)
    {
        if (child == parent)
        {
            return true;
        }
    }

    for (const auto& child : m_Children)
    {
        if (child->IsChild(parent))
        {
            return true;
        }
    }

    return false;
}