#include "TransformComponent.h"
#include "GameObject.h"

using namespace dae;

TransformComponent::TransformComponent(std::weak_ptr<GameObject> pOwner)
    : BaseComponent(pOwner)
    , m_LocalPosition{ 0.f, 0.f, 0.f }
    , m_WorldPosition{ 0.f, 0.f, 0.f}
	, m_Direction{0.f, 0.f}
	, m_RotationAngle{0.f}
{
}

void TransformComponent::Update(float)
{

}

const glm::vec3& TransformComponent::GetLocalPosition() const
{
    return m_LocalPosition;
}

const glm::vec3& TransformComponent::GetWorldPosition()
{
    if (m_PositionIsDirty)
    {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}

const float& TransformComponent::GetRotation()
{
	return m_RotationAngle;
}

const glm::f32vec2& TransformComponent::GetDirection()
{
	return m_Direction;
}

void TransformComponent::Translate(const glm::vec3& translation)
{
	m_LocalPosition += translation;
	SetPositionDirty();
}

void TransformComponent::Rotate(float angle)
{
	m_RotationAngle += angle;
}

const void TransformComponent::SetAngle(const float& angle)
{
	m_RotationAngle = angle;
}

const void TransformComponent::SetLocalPosition(float x, float y, float z)
{
    m_LocalPosition.x = x;
    m_LocalPosition.y = y;
    m_LocalPosition.z = z;

	SetPositionDirty();
}

const void TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;

	SetPositionDirty();
}

const void TransformComponent::SetWorldPosition(const glm::vec3& pos)
{
	m_WorldPosition = pos;
}

const void TransformComponent::SetDirection(const glm::f32vec2& dir)
{
	m_Direction = dir;
}

void TransformComponent::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		auto owner = GetOwner();
		if (owner.lock() == nullptr)
		{
			return;
		}
		auto parent = owner.lock()->GetParent().lock();
		if (!parent)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			auto transform = parent->GetComponent<TransformComponent>();
			if (transform)
			{
				m_WorldPosition = transform->GetWorldPosition() + m_LocalPosition;
			}
		}
		m_PositionIsDirty = false;
	}
	
}

void TransformComponent::SetPositionDirty()
{
	m_PositionIsDirty = true;
}