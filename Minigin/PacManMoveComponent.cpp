#include "PacManMoveComponent.h"
using namespace dae;

PacManMoveComponent::PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed, int beginCellRow, int beginCellCol) :
    BaseComponent(pOwner),
    m_WantedDirection(glm::f32vec2(0.f, 0.f)),
    m_CurrentDirection(glm::f32vec2(0.f, 0.f)),
    m_Speed(speed),
    m_Transform(),
    m_Grid(19, 17, 27, 27, 110, 27),
    m_CurentCellId(),
    m_TargetPosition(glm::vec3(0.0f)),
    m_FollowPath(false)
{
    m_Transform = pOwner.lock()->GetComponent<TransformComponent>();

    m_Grid.setCell(0, 8, CellType::Wall);

    m_Grid.setCell(1, 1, CellType::Wall);
    m_Grid.setCell(1, 2, CellType::Wall);
    m_Grid.setCell(1, 4, CellType::Wall);
    m_Grid.setCell(1, 5, CellType::Wall);
    m_Grid.setCell(1, 6, CellType::Wall);
    m_Grid.setCell(1, 8, CellType::Wall);
    m_Grid.setCell(1, 10, CellType::Wall);
    m_Grid.setCell(1, 11, CellType::Wall);
    m_Grid.setCell(1, 12, CellType::Wall);
    m_Grid.setCell(1, 14, CellType::Wall);
    m_Grid.setCell(1, 15, CellType::Wall);

    m_Grid.setCell(3, 1, CellType::Wall);
    m_Grid.setCell(3, 2, CellType::Wall);
    m_Grid.setCell(3, 4, CellType::Wall);
    m_Grid.setCell(3, 6, CellType::Wall);
    m_Grid.setCell(3, 7, CellType::Wall);
    m_Grid.setCell(3, 8, CellType::Wall);
    m_Grid.setCell(3, 9, CellType::Wall);
    m_Grid.setCell(3, 10, CellType::Wall);
    m_Grid.setCell(3, 12, CellType::Wall);
    m_Grid.setCell(3, 14, CellType::Wall);
    m_Grid.setCell(3, 15, CellType::Wall);

    m_Grid.setCell(4, 4, CellType::Wall);
    m_Grid.setCell(4, 8, CellType::Wall);
    m_Grid.setCell(4, 12, CellType::Wall);

    m_Grid.setCell(5, 0, CellType::Wall);
    m_Grid.setCell(5, 1, CellType::Wall);
    m_Grid.setCell(5, 2, CellType::Wall);
    m_Grid.setCell(5, 4, CellType::Wall);
    m_Grid.setCell(5, 5, CellType::Wall);
    m_Grid.setCell(5, 6, CellType::Wall);
    m_Grid.setCell(5, 8, CellType::Wall);
    m_Grid.setCell(5, 10, CellType::Wall);
    m_Grid.setCell(5, 11, CellType::Wall);
    m_Grid.setCell(5, 12, CellType::Wall);
    m_Grid.setCell(5, 14, CellType::Wall);
    m_Grid.setCell(5, 15, CellType::Wall);
    m_Grid.setCell(5, 16, CellType::Wall);

    m_Grid.setCell(6, 0, CellType::Wall);
    m_Grid.setCell(6, 1, CellType::Wall);
    m_Grid.setCell(6, 2, CellType::Wall);
    m_Grid.setCell(6, 4, CellType::Wall);
    m_Grid.setCell(6, 12, CellType::Wall);
    m_Grid.setCell(6, 14, CellType::Wall);
    m_Grid.setCell(6, 15, CellType::Wall);
    m_Grid.setCell(6, 16, CellType::Wall);

    m_Grid.setCell(7, 0, CellType::Wall);
    m_Grid.setCell(7, 1, CellType::Wall);
    m_Grid.setCell(7, 2, CellType::Wall);
    m_Grid.setCell(7, 4, CellType::Wall);
    m_Grid.setCell(7, 6, CellType::Wall);
    m_Grid.setCell(7, 7, CellType::Wall);
    m_Grid.setCell(7, 8, CellType::Wall);
    m_Grid.setCell(7, 9, CellType::Wall);
    m_Grid.setCell(7, 10, CellType::Wall);
    m_Grid.setCell(7, 12, CellType::Wall);
    m_Grid.setCell(7, 14, CellType::Wall);
    m_Grid.setCell(7, 15, CellType::Wall);
    m_Grid.setCell(7, 16, CellType::Wall);


    m_Grid.setCell(8, 6, CellType::Wall);
    m_Grid.setCell(8, 7, CellType::Wall);
    m_Grid.setCell(8, 8, CellType::Wall);
    m_Grid.setCell(8, 9, CellType::Wall);
    m_Grid.setCell(8, 10, CellType::Wall);

    m_Grid.setCell(9, 0, CellType::Wall);
    m_Grid.setCell(9, 1, CellType::Wall);
    m_Grid.setCell(9, 2, CellType::Wall);
    m_Grid.setCell(9, 4, CellType::Wall);
    m_Grid.setCell(9, 6, CellType::Wall);
    m_Grid.setCell(9, 7, CellType::Wall);
    m_Grid.setCell(9, 8, CellType::Wall);
    m_Grid.setCell(9, 9, CellType::Wall);
    m_Grid.setCell(9, 10, CellType::Wall);
    m_Grid.setCell(9, 12, CellType::Wall);
    m_Grid.setCell(9, 14, CellType::Wall);
    m_Grid.setCell(9, 15, CellType::Wall);
    m_Grid.setCell(9, 16, CellType::Wall);


    m_Grid.setCell(10, 0, CellType::Wall);
    m_Grid.setCell(10, 1, CellType::Wall);
    m_Grid.setCell(10, 2, CellType::Wall);
    m_Grid.setCell(10, 4, CellType::Wall);
    m_Grid.setCell(10, 12, CellType::Wall);
    m_Grid.setCell(10, 14, CellType::Wall);
    m_Grid.setCell(10, 15, CellType::Wall);
    m_Grid.setCell(10, 16, CellType::Wall);

    m_Grid.setCell(11, 0, CellType::Wall);
    m_Grid.setCell(11, 1, CellType::Wall);
    m_Grid.setCell(11, 2, CellType::Wall);
    m_Grid.setCell(11, 4, CellType::Wall);
    m_Grid.setCell(11, 6, CellType::Wall);
    m_Grid.setCell(11, 7, CellType::Wall);
    m_Grid.setCell(11, 8, CellType::Wall);
    m_Grid.setCell(11, 9, CellType::Wall);
    m_Grid.setCell(11, 10, CellType::Wall);
    m_Grid.setCell(11, 12, CellType::Wall);
    m_Grid.setCell(11, 14, CellType::Wall);
    m_Grid.setCell(11, 15, CellType::Wall);
    m_Grid.setCell(11, 16, CellType::Wall);

    m_Grid.setCell(12, 8, CellType::Wall);

    m_Grid.setCell(13, 1, CellType::Wall);
    m_Grid.setCell(13, 2, CellType::Wall);
    m_Grid.setCell(13, 4, CellType::Wall);
    m_Grid.setCell(13, 5, CellType::Wall);
    m_Grid.setCell(13, 6, CellType::Wall);
    m_Grid.setCell(13, 8, CellType::Wall);
    m_Grid.setCell(13, 10, CellType::Wall);
    m_Grid.setCell(13, 11, CellType::Wall);
    m_Grid.setCell(13, 12, CellType::Wall);
    m_Grid.setCell(13, 14, CellType::Wall);
    m_Grid.setCell(13, 15, CellType::Wall);

    m_Grid.setCell(14, 2, CellType::Wall);
    m_Grid.setCell(14, 14, CellType::Wall);

    m_Grid.setCell(15, 0, CellType::Wall);
    m_Grid.setCell(15, 2, CellType::Wall);
    m_Grid.setCell(15, 4, CellType::Wall);
    m_Grid.setCell(15, 6, CellType::Wall);
    m_Grid.setCell(15, 7, CellType::Wall);
    m_Grid.setCell(15, 8, CellType::Wall);
    m_Grid.setCell(15, 9, CellType::Wall);
    m_Grid.setCell(15, 10, CellType::Wall);
    m_Grid.setCell(15, 12, CellType::Wall);
    m_Grid.setCell(15, 14, CellType::Wall);
    m_Grid.setCell(15, 16, CellType::Wall);

    m_Grid.setCell(16, 4, CellType::Wall);
    m_Grid.setCell(16, 8, CellType::Wall);
    m_Grid.setCell(16, 12, CellType::Wall);

    m_Grid.setCell(17, 1, CellType::Wall);
    m_Grid.setCell(17, 2, CellType::Wall);
    m_Grid.setCell(17, 3, CellType::Wall);
    m_Grid.setCell(17, 4, CellType::Wall);
    m_Grid.setCell(17, 5, CellType::Wall);
    m_Grid.setCell(17, 6, CellType::Wall);
    m_Grid.setCell(17, 8, CellType::Wall);
    m_Grid.setCell(17, 10, CellType::Wall);
    m_Grid.setCell(17, 11, CellType::Wall);
    m_Grid.setCell(17, 12, CellType::Wall);
    m_Grid.setCell(17, 13, CellType::Wall);
    m_Grid.setCell(17, 14, CellType::Wall);
    m_Grid.setCell(17, 15, CellType::Wall);

    m_Grid.setRowHeight(3, 13.5);
    m_Grid.setRowHeight(5, 13.5);
    m_Grid.setRowHeight(7, 13.5);
    m_Grid.setRowHeight(9, 13.5);
    m_Grid.setRowHeight(11, 13.5);
    m_Grid.setRowHeight(13, 13.5);
    m_Grid.setRowHeight(15, 13.5);
    m_Grid.setRowHeight(17, 13.5);

    m_Grid.setColWidth(2, 13.5);
    m_Grid.setColWidth(4, 13.5);
    m_Grid.setColWidth(6, 13.5);
    m_Grid.setColWidth(8, 13.5);
    m_Grid.setColWidth(10, 13.5);
    m_Grid.setColWidth(12, 13.5);
    m_Grid.setColWidth(14, 13.5);

    m_CurentCellId = m_Grid.getCellId(beginCellRow, beginCellCol);
    std::pair<float, float> initialCenter = m_Grid.getCellCenter(m_CurentCellId);

    glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
    m_TargetPosition = initialPosition;

    m_Transform->SetLocalPosition(initialPosition);
}

void PacManMoveComponent::Update(float deltaTime)
{
    if (!m_FollowPath)
    {
        if (/*IsOppositeDirection(m_WantedDirection, m_CurrentDirection) ||*/
            glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f)
        {
            if (m_WantedDirection.x != 0 || m_WantedDirection.y != 0) {

                int nextWantedCellId = m_Grid.getNextCellId(m_CurentCellId, m_WantedDirection);

                if (nextWantedCellId != m_CurentCellId) {
                    std::pair<int, int> nextCellPosition = m_Grid.getCellPosition(nextWantedCellId);
                    if (nextCellPosition.first != -1 && nextCellPosition.second != -1) {
                        CellType nextCellType = m_Grid.getCell(nextCellPosition.first, nextCellPosition.second);
                        if (nextCellType != CellType::Wall) {
                            m_CurrentDirection = m_WantedDirection;
                            m_WantedDirection = glm::f32vec2(0.f, 0.f);
                        }
                    }
                }
            }

            int nextCellId = m_Grid.getNextCellId(m_CurentCellId, m_CurrentDirection);

            std::pair<int, int> nextCellPosition = m_Grid.getCellPosition(nextCellId);
            CellType nextCellType = m_Grid.getCell(nextCellPosition.first, nextCellPosition.second);

            if (nextCellType != CellType::Wall) {
                std::pair<float, float> targetCenter = m_Grid.getCellCenter(nextCellId);
                m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
                m_CurentCellId = nextCellId;
            }
            else {
                m_TargetPosition = m_Transform->GetLocalPosition();
            }
        }


        glm::vec3 currentPosition = m_Transform->GetLocalPosition();
        glm::vec3 direction = glm::normalize(m_TargetPosition - currentPosition);
        glm::vec3 displacement = direction * m_Speed * deltaTime;

        if (glm::length(displacement) > glm::length(m_TargetPosition - currentPosition)) {
            displacement = m_TargetPosition - currentPosition;
        }

        if (m_TargetPosition == currentPosition) {
            displacement = glm::vec3(0.f, 0.f, 0.f);
        }

        m_Transform->Translate(displacement);
    }
}

void PacManMoveComponent::Render() const
{
    //m_Grid.drawGrid();
}

void PacManMoveComponent::SetDirection(Movement movement)
{
    glm::f32vec2 direction = glm::f32vec2(0.f, 0.f);

    switch (movement) {
    case Movement::DOWN:
        m_WantedDirection = glm::f32vec2{ 0.f, 1.f };
        break;
    case Movement::RIGHT:
        m_WantedDirection = glm::f32vec2{ 1.f, 0.f };
        break;
    case Movement::UP:
        m_WantedDirection = glm::f32vec2{ 0.f, -1.f };
        break;
    case Movement::LEFT:
        m_WantedDirection = glm::f32vec2{ -1.f, 0.f };
        break;
    default:
        break;
    }

}

void PacManMoveComponent::SetAngle(float angle)
{
    m_Transform->SetAngle(angle);
}

bool PacManMoveComponent::IsOppositeDirection(const glm::f32vec2& dir1, const glm::f32vec2& dir2) {
    return (dir1.x == -dir2.x && dir1.y == dir2.y) || (dir1.y == -dir2.y && dir1.x == dir2.x);
}


void PacManMoveComponent::FollowPath(float deltaTime, int cellIndex)
{
    if (followTargetPath.empty() || followTargetPathIndex >= followTargetPath.size()) {
        followTargetPath = m_Grid.FindPath(m_CurentCellId, cellIndex);
        followTargetPathIndex = 0;
    }

    if (followTargetPathIndex < followTargetPath.size() && glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurentCellId = followTargetPath[followTargetPathIndex++];
        std::pair<float, float> targetCenter = m_Grid.getCellCenter(m_CurentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (followTargetPathIndex >= followTargetPath.size() && glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        followTargetPath.clear();
    }
}

void PacManMoveComponent::FollowSecondBestPath(float deltaTime, int cellIndex)
{
    if (followTargetPath.empty() || followTargetPathIndex >= followTargetPath.size()) {
        followTargetPath = m_Grid.FindSecondBestPath(m_CurentCellId, cellIndex);
        followTargetPathIndex = 0;
    }

    if (followTargetPathIndex < followTargetPath.size() && glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurentCellId = followTargetPath[followTargetPathIndex++];
        std::pair<float, float> targetCenter = m_Grid.getCellCenter(m_CurentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (followTargetPathIndex >= followTargetPath.size() && glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        followTargetPath.clear();
    }
}

void PacManMoveComponent::SetTargetCellIndex(int cellIndex)
{
    followTargetPath.clear();
    followTargetPathIndex = 0;
    followTargetPath = m_Grid.FindPath(m_CurentCellId, cellIndex);
}

void PacManMoveComponent::MoveEntity(float deltaTime) {
    glm::vec3 currentPosition = m_Transform->GetLocalPosition();
    glm::vec3 directionVec = glm::normalize(m_TargetPosition - currentPosition);
    glm::vec3 displacement = directionVec * m_Speed * deltaTime;

    if (glm::length(displacement) > glm::length(m_TargetPosition - currentPosition)) {
        displacement = m_TargetPosition - currentPosition;
    }

    if (m_TargetPosition == currentPosition) {
        displacement = glm::vec3(0.f, 0.f, 0.f);
    }

    m_Transform->Translate(displacement);

    glm::f32vec2 direction2D = glm::normalize(glm::f32vec2(directionVec.x, directionVec.y));
    m_Transform->SetDirection(direction2D);

    if (glm::abs(directionVec.x) > glm::abs(directionVec.y)) {
        if (directionVec.x > 0) {
            SetDirection(Movement::RIGHT);
            m_CurrentDirection = m_WantedDirection;
        }
        else {
            SetDirection(Movement::LEFT);
            m_CurrentDirection = m_WantedDirection;
        }
    }
    else {
        if (directionVec.y > 0) {
            SetDirection(Movement::DOWN);
            m_CurrentDirection = m_WantedDirection;
        }
        else {
            SetDirection(Movement::UP);
            m_CurrentDirection = m_WantedDirection;
        }
    }
}

bool PacManMoveComponent::HasReachedTargetCell() const {
    if (followTargetPathIndex >= followTargetPath.size())
        return true;

    glm::vec3 currentPosition = m_Transform->GetLocalPosition();
    std::pair<float, float> nextCellCenter = m_Grid.getCellCenter(followTargetPath[followTargetPathIndex]);
    glm::vec3 nextPosition = glm::vec3(nextCellCenter.first - 7.f, nextCellCenter.second - 8.f, 0.0f);

    float distance = glm::length(currentPosition - nextPosition);
    float threshold = 0.1f;

    return distance < threshold;
}


void PacManMoveComponent::SetFollowPath(bool followPath)
{
    m_FollowPath = followPath;
}

