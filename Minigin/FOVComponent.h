#pragma once
#include "BaseComponent.h"
#include <vector>
#include <algorithm>
#include "GameObject.h"

namespace dae
{
    class FOVComponent : public BaseComponent
    {
    public:
        FOVComponent(std::weak_ptr<GameObject> owner, float viewAngle, float viewDistance)
            : BaseComponent(owner), m_ViewAngle(viewAngle), m_ViewDistance(viewDistance), m_PlayerInSight(false), m_Angle(0.f) {}

        void Update(float) override
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
            float closestDistance = std::numeric_limits<float>::max();

            auto ownerTransform = owner->GetComponent<TransformComponent>();
            if (!ownerTransform)
                return;

            glm::vec2 ownerPosition = ownerTransform->GetWorldPosition();

            for (auto& gameObject : gameObjects)
            {
                if (gameObject == owner)
                    continue;

                if (gameObject->GetTag() == "Player")
                {
                    auto targetTransform = gameObject->GetComponent<TransformComponent>();
                    if (!targetTransform)
                        continue;

                    glm::vec2 targetPosition = targetTransform->GetWorldPosition();
                    glm::vec2 direction = glm::normalize(targetPosition - ownerPosition);
                    float angleToTarget = std::atan2(direction.y, direction.x) * 180 / float(M_PI);

                    float dx = targetPosition.x - ownerPosition.x;
                    float dy = targetPosition.y - ownerPosition.y;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance <= m_ViewDistance && std::abs(angleToTarget - m_Angle) <= m_ViewAngle / 2)
                    {
                        m_PlayerInSight = true;
                        if (distance < closestDistance)
                        {
                            closestDistance = distance;
                            m_PlayerInVision = gameObject;
                        }
                    }
                }
            }
        }

        /*void Render() const override
        {
            auto owner = GetOwner().lock();
            if (!owner)
                return;

            auto ownerTransform = owner->GetComponent<TransformComponent>();
            if (!ownerTransform)
                return;

            auto renderer = Renderer::GetInstance().GetSDLRenderer();
            if (!renderer)
                return;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);

            std::vector<SDL_Point> fovVertices;
            fovVertices.push_back({ static_cast<int>(ownerTransform->GetWorldPosition().x + 7.f), static_cast<int>(ownerTransform->GetWorldPosition().y + 8.f) });

            float startAngle = m_Angle - m_ViewAngle / 2;
            float endAngle = m_Angle + m_ViewAngle / 2;

            const int numSegments = 120;
            const float angleStep = m_ViewAngle / numSegments;

            for (int i = 0; i <= numSegments; ++i)
            {
                float angle = startAngle + angleStep * i;
                if (angle > endAngle)
                    break;
                double x = ownerTransform->GetWorldPosition().x + m_ViewDistance * std::cos(angle * M_PI / 180.0f);
                double y = ownerTransform->GetWorldPosition().y + m_ViewDistance * std::sin(angle * M_PI / 180.0f);
                fovVertices.push_back({ static_cast<int>(x), static_cast<int>(y) });
            }

            SDL_RenderDrawLines(renderer, fovVertices.data(), static_cast<int>(fovVertices.size()));

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }*/

        bool GetPlayerInSight() const
        {
            return m_PlayerInSight;
        }

        std::shared_ptr<GameObject> GetPlayerInVision() const
        {
            return m_PlayerInVision.lock();
        }
    private:
        float m_ViewDistance;
        float m_ViewAngle;
        bool m_PlayerInSight;
        float m_Angle;
        std::weak_ptr<GameObject> m_PlayerInVision;
    };
}

