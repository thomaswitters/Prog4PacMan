#include "BoxColliderComponent.h"

std::vector<std::weak_ptr<dae::GameObject>> dae::BoxColliderComponent::m_pCollidableObjects;

dae::BoxColliderComponent::BoxColliderComponent(std::weak_ptr<GameObject> pOwner, float width, float height, const glm::vec2& offset) :
    BaseComponent(pOwner),
    m_Width(width),
    m_Height(height),
    m_Offset(offset),
    m_IsOverlapped(false),
    m_IsTrigger(true),
    m_IsActive(true)
{
    m_pTransformComponent = pOwner.lock()->GetComponent<TransformComponent>();
    m_pCollidableObjects.push_back(pOwner);
    FetchColliders();
}

void dae::BoxColliderComponent::FetchColliders()
{
    m_pColliders.clear();
    auto pOwner = GetOwner().lock();
    if (!pOwner) return;
    for (auto& obj : m_pCollidableObjects)
    {
        auto otherOwner = obj.lock();
        if (!otherOwner || otherOwner == pOwner)
            continue;

        auto collider = otherOwner->GetComponent<BoxColliderComponent>();
        if (!collider)
            continue;

        m_pColliders.push_back(collider);
    }
}

void dae::BoxColliderComponent::Render() const
{
    /* glm::vec2 minBounds = GetMinBounds();
     glm::vec2 maxBounds = GetMaxBounds();

     dae::Renderer::GetInstance().DrawRectangle(minBounds.x, minBounds.y, m_Width, m_Height, { 255, 255, 255, 255 });*/
}

bool dae::BoxColliderComponent::CheckCollision(const std::shared_ptr<BoxColliderComponent>& otherCollider) const
{
    if (!m_IsActive || !otherCollider || !otherCollider->IsActive())
    {
        return false;
    }

    glm::vec2 thisMin = GetMinBounds();
    glm::vec2 thisMax = GetMaxBounds();
    glm::vec2 otherMin = otherCollider->GetMinBounds();
    glm::vec2 otherMax = otherCollider->GetMaxBounds();

    bool overlapX = thisMin.x < otherMax.x && thisMax.x > otherMin.x;
    bool overlapY = thisMin.y < otherMax.y && thisMax.y > otherMin.y;

    return overlapX && overlapY;
}

glm::vec2 dae::BoxColliderComponent::GetMinBounds() const
{
    if (m_pTransformComponent)
    {
        glm::vec2 position = m_pTransformComponent->GetLocalPosition();
        return position - m_Offset - glm::vec2(m_Width / 2.0f, m_Height / 2.0f);

    }
    return glm::vec2(0.f, 0.f);
}
glm::vec2 dae::BoxColliderComponent::GetMaxBounds() const
{
    if (m_pTransformComponent)
    {
        glm::vec2 position = m_pTransformComponent->GetLocalPosition();
        return position - m_Offset + glm::vec2(m_Width / 2.0f, m_Height / 2.0f);
    }
    return glm::vec2(0.f, 0.f);
}

std::vector<std::weak_ptr<dae::GameObject>> dae::BoxColliderComponent::GetCollidingObjects() const
{
    std::vector<std::weak_ptr<GameObject>> collidingObjects;
    auto pOwner = GetOwner().lock();
    for (auto& collider : m_pColliders)
    {
        auto otherCollider = collider.lock();
        if (!otherCollider || otherCollider->GetOwner().lock() == pOwner)
            continue;

        if (CheckCollision(otherCollider))
        {
            collidingObjects.push_back(otherCollider->GetOwner());
        }
    }
    return collidingObjects;
}

void dae::BoxColliderComponent::Update(float)
{
    m_IsOverlapped = false;
    for (auto& collider : m_pColliders)
    {
        auto otherCollider = collider.lock();
        if (!otherCollider)
            continue;

        if (CheckCollision(otherCollider))
        {
            m_IsOverlapped = true;
        }
    }
}