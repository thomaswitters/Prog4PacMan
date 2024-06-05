#include "PacManMoveComponent.h"

using namespace dae;

PacManMoveComponent::PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed) :
    BaseComponent(pOwner),
    m_Direction(glm::f32vec2(1.f, 0.f)),
    m_Speed(speed),
    m_Transform()
{
    m_Transform = pOwner.lock()->GetComponent<TransformComponent>();
}

void PacManMoveComponent::Update(float deltaTime)
{
    glm::f32vec3 newPosition = m_Transform->GetLocalPosition() + glm::f32vec3(m_Direction.x, m_Direction.y, 0.0f) * deltaTime * m_Speed;

    m_Transform->SetLocalPosition(newPosition);
}

void PacManMoveComponent::SetDirection(glm::f32vec2 direction)
{
    m_Direction = direction;
    m_Transform->SetDirection(direction);
}

void PacManMoveComponent::SetAngle(float angle)
{
    m_Transform->SetAngle(angle);
}