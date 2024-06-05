#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Graph.h"

#include <glm/gtc/matrix_transform.hpp>

namespace dae
{
    class PacManMoveComponent final : public BaseComponent
    {
    public:
        enum class Movement
        {
            LEFT, RIGHT, UP, DOWN, NONE
        };

        PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed);
        virtual ~PacManMoveComponent() = default;

        PacManMoveComponent(const PacManMoveComponent&) = delete;
        PacManMoveComponent(PacManMoveComponent&&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&&) = delete;

        void Update(float deltaTime) override;
        void Render() const override;
        void SetDirection(Movement direction);

        void SetAngle(float angle);
    private:
        float m_Speed;
        glm::f32vec2 m_CurrentDirection;
        glm::f32vec2 m_PreviusDirection;
        std::shared_ptr<TransformComponent> m_Transform;

        Grid m_Grid;
        int m_CurentCellId;
        glm::vec3 m_TargetPosition;
    };
}




