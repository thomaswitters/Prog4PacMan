#include "FOVComponent.h"
#include "Scene.h"
#include <limits>
#include <cmath>

namespace dae
{
    FOVComponent::FOVComponent(std::weak_ptr<GameObject> pOwner, float viewAngle, float viewDistance)
        : BaseComponent(pOwner), m_ViewAngle(viewAngle), m_ViewDistance(viewDistance), m_PlayerInSight(false), m_Angle(0.f)
    {
    }

    void FOVComponent::Update(float)
    {
        auto pOwner = GetOwner().lock();
        if (!pOwner)
            return;

        auto pScene = pOwner->GetScene();
        if (!pScene)
            return;

        auto gameObjects = pScene->GetObjects();

        m_PlayerInSight = false;
        m_pPlayerInVision.reset();
        float closestDistanceSquared = std::numeric_limits<float>::max();

        if (!m_pOwnerTransform)
        {
            m_pOwnerTransform = pOwner->GetComponent<TransformComponent>();
            if (!m_pOwnerTransform)
                return;
        }

        glm::vec2 ownerPosition = m_pOwnerTransform->GetWorldPosition();

        float cosHalfViewAngle = std::cos(glm::radians(m_ViewAngle / 2.0f));
        glm::vec2 ownerForward = glm::vec2(std::cos(glm::radians(m_Angle)), std::sin(glm::radians(m_Angle)));

        for (const auto& pGameObject : gameObjects)
        {
            if (pGameObject == pOwner || pGameObject->GetTag() != "Player")
                continue;

            m_pPlayerTransform = pGameObject->GetComponent<TransformComponent>();
            if (!m_pPlayerTransform)
                continue;

            glm::vec2 targetPosition = m_pPlayerTransform->GetWorldPosition();
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
                    m_pPlayerInVision = pGameObject;
                }
            }
        }
    }

    bool FOVComponent::IsPlayerInSight() const
    {
        return m_PlayerInSight;
    }

    std::shared_ptr<GameObject> FOVComponent::GetPlayerInVision() const
    {
        return m_pPlayerInVision.lock();
    }
}