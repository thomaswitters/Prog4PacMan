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

        PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed, int beginCellRow, int beginCellCol);
        virtual ~PacManMoveComponent() = default;

        PacManMoveComponent(const PacManMoveComponent&) = delete;
        PacManMoveComponent(PacManMoveComponent&&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&) = delete;
        PacManMoveComponent& operator= (const PacManMoveComponent&&) = delete;

        void Update(float deltaTime) override;
        void Render() const override;
        void SetDirection(Movement direction);

        void SetAngle(float angle);

        bool IsOppositeDirection(const glm::f32vec2& dir1, const glm::f32vec2& dir2);


        void SetFollowPath(bool followPath);
        void FollowPath(float deltaTime, int cellIndex);
        void FollowSecondBestPath(float deltaTime, int cellIndex);

        void SetTargetCellIndex(int cellIndex);

        int GetCurrentIndex() const
        {
            return m_CurentCellId;
        }
        void SetCurrentIndex(int idx) { m_CurentCellId = idx; }
        int GetCurrentGraphCellId() { return m_Grid.getCellId(m_Transform->GetLocalPosition()); }
        void MoveEntity(float deltaTime);
        bool HasReachedTargetCell() const;

    private:
        float m_Speed;
        glm::f32vec2 m_WantedDirection;
        glm::f32vec2 m_CurrentDirection;
        std::shared_ptr<TransformComponent> m_Transform;

        Grid m_Grid;
        int m_CurentCellId;
        glm::vec3 m_TargetPosition;
        bool m_FollowPath;

        std::vector<int> followTargetPath;
        size_t followTargetPathIndex;
    };
}


