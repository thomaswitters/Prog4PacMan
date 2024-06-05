#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Graph.h"
#include "StateManagerMovement.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <fstream>
#include <json.hpp>
#include "BoxColliderComponent.h"

namespace dae
{
    class PacManMoveComponent final : public BaseComponent
    {
    public:
        enum class Movement
        {
            LEFT, RIGHT, UP, DOWN, NONE
        };

        PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed, int beginCellRow, int beginCellCol, std::shared_ptr<StateManagerMovement> stateManagerMovement = nullptr);
        virtual ~PacManMoveComponent() = default;

        // Delete copy and move constructors and assignment operators
        PacManMoveComponent(const PacManMoveComponent&) = delete;
        PacManMoveComponent(PacManMoveComponent&&) = delete;
        PacManMoveComponent& operator=(const PacManMoveComponent&) = delete;
        PacManMoveComponent& operator=(PacManMoveComponent&&) = delete;

        // Core component functions
        void Update(float deltaTime) override;
        void Render() const override;

        // Movement control
        void SetDirection(Movement direction);
        void SetSpeed(float speed);
        void SetAngle(float angle);
        bool IsOppositeDirection(const glm::vec2& dir1, const glm::vec2& dir2);

        // Path following
        void SetFollowPath(bool followPath);
        void FollowPathHelper(float deltaTime, int cellIndex, std::vector<int>& path, size_t& pathIndex, std::vector<int>(Grid::* findPathFunction)(int, int));
        void FollowPath(float deltaTime, int cellIndex);
        void FollowPathToPlayer(float deltaTime, int cellIndex);
        void FleeFromPlayer(float deltaTime, int playerCellIndex);
        void FollowSecondBestPath(float deltaTime, int cellIndex);
        void FollowToPathThatPacManWillBeAt(float deltaTime, int cellIndex, glm::vec2 currentDirection);

        // Target cell management
        void SetTargetCellIndex(int cellIndex);
        int GetCurrentIndex() const { return m_CurrentCellId; }
        void SetCurrentIndex(int idx) { m_CurrentCellId = idx; }
        int GetCurrentGraphCellId() { return m_Grid.getCellId(m_Transform->GetLocalPosition()); }
        bool HasReachedTargetCell() const;

        // State management
        void SetStateManager(std::shared_ptr<StateManagerMovement> stateManager) { m_StateManagerMovement = stateManager; }
        std::shared_ptr<StateManagerMovement> GetStateManager() const { return m_StateManagerMovement; }

        // Movement control
        void MoveEntity(float deltaTime);
        void Respawn(std::shared_ptr<BoxColliderComponent> boxCollider);
        void RespawnGhost();

        glm::vec2 GetCurrentDirection() const { return m_CurrentDirection; }
    private:
        nlohmann::json LoadJson(const std::string& filename);

        // Member variables
        float m_Speed;
        glm::vec2 m_WantedDirection;
        glm::vec2 m_CurrentDirection;
        std::shared_ptr<TransformComponent> m_Transform;

        Grid m_Grid;
        int m_CurrentCellId;
        int m_LastCellIndex;
        glm::vec3 m_TargetPosition;
        bool m_FollowPath;

        std::vector<int> followTargetPath;
        size_t followTargetPathIndex;

        glm::vec2 m_PreviousDirection = glm::vec2(0.0f, 0.0f);

        std::shared_ptr<StateManagerMovement> m_StateManagerMovement;
        
    };
}


