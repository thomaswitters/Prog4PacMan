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
			: BaseComponent(owner), m_ViewAngle(viewAngle), m_ViewDistance(viewDistance), m_PlayerInSight(false), m_Angle(0.f){}

		void Update(float) override
		{
            auto owner = GetOwner().lock();
            if (!owner)
                return;

            auto scene = owner->GetScene();
            if (!scene)
                return;

            auto gameObjects = scene->GetObjects();

            for (auto& gameObject : gameObjects)
            {
                if (gameObject == owner)
                    continue;

                if (gameObject->GetTag() == "Player")
                {
                    auto targetTransform = gameObject->GetComponent<TransformComponent>();
                    auto ownerTransform = owner->GetComponent<TransformComponent>();
                    if (!targetTransform)
                        continue;

                    if (!ownerTransform)
                        continue;

                    float dx = targetTransform->GetWorldPosition().x - ownerTransform->GetWorldPosition().x;
                    float dy = targetTransform->GetWorldPosition().y - ownerTransform->GetWorldPosition().y;

                    double angleToTarget = std::atan2(dy, dx) * 180 / M_PI;

                    if (angleToTarget < 0)
                        angleToTarget += 360;

                    ownerTransform->GetDirection();
                    if (ownerTransform->GetDirection() == glm::f32vec2{ 1.f, 0.f })
                    {
                        m_Angle = 0.f;
                    }
                    else if(ownerTransform->GetDirection() == glm::f32vec2{ 0.f, 1.f })
                    {
                        m_Angle = 90.f;
                    }
                    else if (ownerTransform->GetDirection() == glm::f32vec2{ -1.f, 0.f })
                    {
                        m_Angle = 180.f;
                    }
                    else if (ownerTransform->GetDirection() == glm::f32vec2{ 0.f, -1.f })
                    {
                        m_Angle = 270.f;
                    }
                    // glm::f32vec2{ 1.f, 0.f } = agle 0 degrees
                    // glm::f32vec2{ 0.f, 1.f } = agle 90 degrees
                    // glm::f32vec2{ -1.f, 0.f } = agle 180 degrees
                    // glm::f32vec2{ 0.f, -1.f } = agle 270 degrees

                    double angleDifference = std::abs(/*ownerTransform->GetRotation()*/m_Angle - angleToTarget);

                    if (angleDifference > 180)
                        angleDifference = 360 - angleDifference;

                    if (angleDifference <= m_ViewAngle / 2 &&
                        std::sqrt(dx * dx + dy * dy) <= m_ViewDistance)
                    {
                        m_PlayerInSight = true;
                    }
                    else
                    {
                        m_PlayerInSight = false;
                    }
                }
            }
		}
        //just render to test
		void Render() const override
		{
            //Just to test
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
            fovVertices.push_back({ static_cast<int>(ownerTransform->GetWorldPosition().x + 7.f), static_cast<int>(ownerTransform->GetWorldPosition().y + 8.f)});

            float startAngle =/* ownerTransform->GetRotation()*/m_Angle - m_ViewAngle / 2;
            float endAngle = /*ownerTransform->GetRotation()*/m_Angle + m_ViewAngle / 2;


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
		}

        bool GetPlayerInSight()
        {
            return m_PlayerInSight;
        }
	private:
        float m_ViewDistance;
		float m_ViewAngle;
        bool m_PlayerInSight;
        float m_Angle;
	};
}

