#include "FOVComponent.h"
#include "Scene.h"
#include <limits>
#include <cmath>

namespace dae
{
    FOVComponent::FOVComponent(std::weak_ptr<GameObject> owner, float viewAngle, float viewDistance)
        : BaseComponent(owner), m_ViewAngle(viewAngle), m_ViewDistance(viewDistance), m_PlayerInSight(false), m_Angle(0.f)
    {
    }

    void FOVComponent::Update(float)
    {
        auto owner = GetOwner().lock();
        if (!owner)
            return;

        auto scene = owner->GetScene();
        if (!scene)
            return;

        auto gameObjects = scene->GetObjects();

        m_PlayerInSight = false;
        m_PlayerInVision.reset();
        float closestDistanceSquared = std::numeric_limits<float>::max();

        if (!m_OwnerTransform)
        {
            m_OwnerTransform = owner->GetComponent<TransformComponent>();
            if (!m_OwnerTransform)
                return;
        }

        glm::vec2 ownerPosition = m_OwnerTransform->GetWorldPosition();

        float cosHalfViewAngle = std::cos(glm::radians(m_ViewAngle / 2.0f));
        glm::vec2 ownerForward = glm::vec2(std::cos(glm::radians(m_Angle)), std::sin(glm::radians(m_Angle)));

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner || gameObject->GetTag() != "Player")
                continue;

            m_PlayerTransform = gameObject->GetComponent<TransformComponent>();
            if (!m_PlayerTransform)
                continue;

            glm::vec2 targetPosition = m_PlayerTransform->GetWorldPosition();
            glm::vec2 direction = targetPosition - ownerPosition;
            float distanceSquared = glm::dot(direction, direction);

            if (distanceSquared > m_ViewDistance * m_ViewDistance)
                continue;

            glm::vec2 directionNormalized = glm::normalize(direction);
            float dotProduct = glm::dot(ownerForward, directionNormalized);

            if (dotProduct >= cosHalfViewAngle)
            {
                m_PlayerInSight = true;
                if (distanceSquared < closestDistanceSquared)
                {
                    closestDistanceSquared = distanceSquared;
                    m_PlayerInVision = gameObject;
                }
            }
        }
    }

    bool FOVComponent::GetPlayerInSight() const
    {
        return m_PlayerInSight;
    }

    std::shared_ptr<GameObject> FOVComponent::GetPlayerInVision() const
    {
        return m_PlayerInVision.lock();
    }
}