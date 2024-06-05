#pragma once

#include "BaseComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace dae
{
    class PacManMoveComponent final : public BaseComponent
    {
    public:
        PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed);
        virtual ~PacManMoveComponent() = default;

        PacManMoveComponent(const PacManMoveComponent&) = delete;
        PacManMoveComponent(PacManMoveComponent&&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&&) = delete;

        void Update(float deltaTime) override;
        void SetDirection(glm::f32vec2 direction);
        void SetAngle(float angle);
    private:
        float m_Speed;
        glm::f32vec2 m_Direction;
        std::shared_ptr<TransformComponent> m_Transform;
    };
}

