#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Render() const
{
    for (const auto& component : m_pComponents)
    {
        component->Render();
    }
}

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
    m_pComponents.push_back(component);
}

void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> component)
{
    auto it = std::find(m_pComponents.begin(), m_pComponents.end(), component);
    if (it != m_pComponents.end())
    {
        m_pComponents.erase(it);
    }
}

bool dae::GameObject::HasComponent(const std::shared_ptr<BaseComponent>& component) const
{
    return std::find(m_pComponents.begin(), m_pComponents.end(), component) != m_pComponents.end();
}

void dae::GameObject::Update(float deltaTime)
{
    for (const auto& component : m_pComponents)
    {
        component->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate()
{
    for (const auto& component : m_pComponents)
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
            for (auto& child : m_pChildren)
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
    auto foundObject = std::find(m_pChildren.begin(), m_pChildren.end(), child);
    if (foundObject != m_pChildren.end())
    {
        m_pChildren.erase(foundObject);
    }
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    m_pChildren.emplace_back(child);
}

bool dae::GameObject::IsChild(const std::shared_ptr<GameObject>& parent) const
{
    if (!parent)
    {
        return false;
    }

    for (const auto& child : m_pChildren)
    {
        if (child == parent)
        {
            return true;
        }
    }

    for (const auto& child : m_pChildren)
    {
        if (child->IsChild(parent))
        {
            return true;
        }
    }

    return false;
}

bool dae::GameObject::HasTag(const std::string& tag) const
{
    return m_Tag == tag;
}

void dae::GameObject::SetScene(Scene* scene)
{
    m_pScene = scene;
    for (const auto& children : m_pChildren)
    {
        children->SetScene(scene);
    }
}

dae::Scene* dae::GameObject::GetScene() const
{
    return m_pScene;
}

void dae::GameObject::RemoveObject()
{
    m_RemoveObject = true;
}

bool dae::GameObject::GetRemoveObject()
{
    return m_RemoveObject;
}