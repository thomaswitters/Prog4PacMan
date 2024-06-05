#include "RotationComponent.h"
#include "GameObject.h"

using namespace dae;

RotationComponent::RotationComponent(std::weak_ptr<GameObject> pOwner, float rotateSpeed, bool rotateClockwise) :
	BaseComponent(pOwner),
	m_Angle(0.f),
	m_Radius(25.f),
	m_RotationSpeed(rotateSpeed),
    m_RotateClockwise(rotateClockwise),
    m_StartPos(),
    m_HasStartPos(false)
{
  
}

void RotationComponent::Update(float deltaTime)
{
    auto owner = GetOwner().lock().get();
    auto transform = owner->GetComponent<TransformComponent>();

    if (!m_HasStartPos)
    {
        m_StartPos.x = transform->GetLocalPosition().x;
        m_StartPos.y = transform->GetLocalPosition().y;
        m_HasStartPos = true;
    }
    

    if (transform)
    {
        // Get current position
        //float currentPosX = transform->GetLocalPosition().x;
        //float currentPosY = transform->GetLocalPosition().y;

        float angularSpeed = m_RotationSpeed / m_Radius;
        float rotationDirection = m_RotateClockwise ? 1.0f : -1.0f;
        m_Angle += rotationDirection * angularSpeed * deltaTime;

        // Calculate new position relative to the current position
        float newX = m_StartPos.x + m_Radius * std::cos(m_Angle);
        float newY = m_StartPos.y + m_Radius * std::sin(m_Angle);

        // Set the new position
        transform->SetLocalPosition({ newX, newY, 0.f });
    }
	
}