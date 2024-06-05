#include "TransformComponent.h"
#include "GameObject.h"

using namespace dae;

TransformComponent::TransformComponent(std::weak_ptr<GameObject> pOwner)
    : BaseComponent(pOwner)
    , m_Position{ 0.f, 0.f, 0.f }
{
}

void TransformComponent::Update(float)
{
    
}

const glm::vec3& TransformComponent::GetPosition() const
{
    return m_Position;
}

void TransformComponent::SetPosition(float x, float y, float z)
{
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;
}
