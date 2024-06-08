#include "PacManMoveComponent.h"
#include <thread>
#include <fstream>

using namespace dae;

PacManMoveComponent::PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed, int beginCellRow, int beginCellCol, std::shared_ptr<StateManagerMovement> stateManagerMovement) :
    BaseComponent(pOwner),
    m_WantedDirection(glm::vec2(0.f, 0.f)),
    m_CurrentDirection(glm::vec2(0.f, 0.f)),
    m_Speed(speed),
    m_pTransform(),
    m_Grid(19, 17, 27, 27, 110, 27),
    m_CurrentCellId(),
    m_LastCellIndex(),
    m_FollowTargetPathIndex(),
    m_TargetPosition(glm::vec3(0.0f)),
    m_FollowPath(false),
    m_StateManagerMovement(stateManagerMovement)
{
    m_pTransform = pOwner.lock()->GetComponent<TransformComponent>();

    nlohmann::json gridConfig = LoadJson("../data/gridConfig.json");

    // Set walls
    for (const auto& wall : gridConfig["grid"]) {
        int row = wall["row"];
        int col = wall["col"];
        std::string type = wall["type"];
        if (type == "Wall") {
            m_Grid.SetCell(row, col, CellType::Wall);
        }
    }

    // Set row heights
    for (const auto& rowHeight : gridConfig["rowHeights"]) {
        int row = rowHeight["row"];
        float height = rowHeight["height"];
        m_Grid.SetRowHeight(row, height);
    }

    // Set column widths
    for (const auto& colWidth : gridConfig["colWidths"]) {
        int col = colWidth["col"];
        float width = colWidth["width"];
        m_Grid.SetColWidth(col, width);
    }

    m_CurrentCellId = m_Grid.GetCellId(beginCellRow, beginCellCol);
    std::pair<float, float> initialCenter = m_Grid.GetCellCenter(m_CurrentCellId);

    glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
    m_TargetPosition = initialPosition;

    m_pTransform->SetLocalPosition(initialPosition);
}

nlohmann::json PacManMoveComponent::LoadJson(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json jsonData;
    file >> jsonData;
    return jsonData;
}

void PacManMoveComponent::Update(float deltaTime)
{
    if (!m_FollowPath)
    {
        if (glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f)
        {
            if (m_WantedDirection.x != 0 || m_WantedDirection.y != 0) {

                int nextWantedCellId = m_Grid.GetNextCellId(m_CurrentCellId, m_WantedDirection);

                if (nextWantedCellId != m_CurrentCellId) {
                    std::pair<int, int> nextCellPosition = m_Grid.GetCellPosition(nextWantedCellId);
                    if (nextCellPosition.first != -1 && nextCellPosition.second != -1) {
                        CellType nextCellType = m_Grid.GetCell(nextCellPosition.first, nextCellPosition.second);
                        if (nextCellType != CellType::Wall) {
                            m_CurrentDirection = m_WantedDirection;
                            m_WantedDirection = glm::vec2(0.f, 0.f);

                            if (GetOwner().lock()->GetTag() == "Player")
                            {
                                if (m_CurrentDirection == glm::vec2{ -1.f, 0.f })
                                {
                                    m_pTransform->SetAngle(180);
                                }
                                else  if (m_CurrentDirection == glm::vec2{ 0.f, 1.f })
                                {
                                    m_pTransform->SetAngle(90);
                                }
                                else  if (m_CurrentDirection == glm::vec2{ 0.f, -1.f })
                                {
                                    m_pTransform->SetAngle(270);
                                }
                                else  if (m_CurrentDirection == glm::vec2{ 1.f, 0.f })
                                {
                                    m_pTransform->SetAngle(0);
                                }
                            }
                        }
                    }
                }
            }

            int nextCellId = m_Grid.GetNextCellId(m_CurrentCellId, m_CurrentDirection);

            std::pair<int, int> nextCellPosition = m_Grid.GetCellPosition(nextCellId);
            CellType nextCellType = m_Grid.GetCell(nextCellPosition.first, nextCellPosition.second);

            if (nextCellType != CellType::Wall) {
                std::pair<float, float> targetCenter = m_Grid.GetCellCenter(nextCellId);
                m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
                m_CurrentCellId = nextCellId;
            }
            else {
                m_TargetPosition = m_pTransform->GetLocalPosition();
            }
        }


        glm::vec3 currentPosition = m_pTransform->GetLocalPosition();
        glm::vec3 direction = glm::normalize(m_TargetPosition - currentPosition);
        glm::vec3 displacement = direction * m_Speed * deltaTime;

        if (glm::length(displacement) > glm::length(m_TargetPosition - currentPosition)) {
            displacement = m_TargetPosition - currentPosition;
        }

        if (m_TargetPosition == currentPosition) {
            displacement = glm::vec3(0.f, 0.f, 0.f);
        }

        m_pTransform->Translate(displacement);


        //respawn player
        if (m_IsRespawning)
        {
            m_RespawnTimer += deltaTime;

            if (m_RespawnTimer >= m_RespawnDelay)
            {
                m_pBoxCollider->SetActive(true);
                m_IsRespawning = false;
            }
        }
    }

    if (m_CurrentCellId == 136)
    {
        m_CurrentCellId = m_Grid.GetCellId(8, 16);
        std::pair<float, float> initialCenter = m_Grid.GetCellCenter(m_CurrentCellId);

        glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
        m_TargetPosition = initialPosition;

        m_pTransform->SetLocalPosition(initialPosition);
    }
    else if (m_CurrentCellId == 152)
    {
        m_CurrentCellId = m_Grid.GetCellId(8, 0);
        std::pair<float, float> initialCenter = m_Grid.GetCellCenter(m_CurrentCellId);

        glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
        m_TargetPosition = initialPosition;

        m_pTransform->SetLocalPosition(initialPosition);
    }
}

void PacManMoveComponent::Render() const
{
    //m_Grid.drawGrid();
}

void PacManMoveComponent::SetDirection(Movement movement)
{
    glm::vec2 direction = glm::vec2(0.f, 0.f);

    switch (movement) {
    case Movement::DOWN:
        m_WantedDirection = glm::vec2{ 0.f, 1.f };
        break;
    case Movement::RIGHT:
        m_WantedDirection = glm::vec2{ 1.f, 0.f };
        break;
    case Movement::UP:
        m_WantedDirection = glm::vec2{ 0.f, -1.f };
        break;
    case Movement::LEFT:
        m_WantedDirection = glm::vec2{ -1.f, 0.f };
        break;
    default:
        break;
    }

}

void PacManMoveComponent::SetAngle(float angle)
{
    m_pTransform->SetAngle(angle);
}

void PacManMoveComponent::Respawn(std::shared_ptr<BoxColliderComponent> boxCollider)
{
    m_CurrentCellId = m_Grid.GetCellId(14, 8);
    std::pair<float, float> initialCenter = m_Grid.GetCellCenter(m_CurrentCellId);

    glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
    m_TargetPosition = initialPosition;

    m_pTransform->SetLocalPosition(initialPosition);
    m_CurrentDirection = glm::vec2{ 0.f, 0.f };

    m_pBoxCollider = boxCollider;
    m_IsRespawning = true;
    m_RespawnTimer = 0.0f;
}

void PacManMoveComponent::RespawnGhost()
{
    m_CurrentCellId = m_Grid.GetCellId(8, 8);
    std::pair<float, float> initialCenter = m_Grid.GetCellCenter(m_CurrentCellId);

    glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
    m_TargetPosition = initialPosition;

    m_pTransform->SetLocalPosition(initialPosition);
    m_CurrentDirection = glm::vec2{ 0.f, 0.f };
}

bool PacManMoveComponent::IsOppositeDirection(const glm::vec2& dir1, const glm::vec2& dir2) {
    return (dir1.x == -dir2.x && dir1.y == dir2.y) || (dir1.y == -dir2.y && dir1.x == dir2.x);
}

void PacManMoveComponent::FollowPathHelper(float deltaTime, int cellIndex, std::vector<int>& path, size_t& pathIndex, std::vector<int>(Grid::* findPathFunction)(int, int)) {
    if (path.empty() || pathIndex >= path.size()) {
        path = (m_Grid.*findPathFunction)(m_CurrentCellId, cellIndex);
        pathIndex = 0;
    }

    if (pathIndex < path.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurrentCellId = path[pathIndex++];
        std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (pathIndex >= path.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        path.clear();
    }
}

void PacManMoveComponent::FollowPath(float deltaTime, int cellIndex) {
    FollowPathHelper(deltaTime, cellIndex, m_FollowTargetPath, m_FollowTargetPathIndex, &Grid::FindPath);
}

void PacManMoveComponent::FollowSecondBestPath(float deltaTime, int cellIndex) {
    FollowPathHelper(deltaTime, cellIndex, m_FollowTargetPath, m_FollowTargetPathIndex, &Grid::FindSecondBestPath);
}

void PacManMoveComponent::FollowToPathThatPacManWillBeAt(float deltaTime, int cellIndex, glm::vec2 currentDirection)
{
    if (m_FollowTargetPath.empty() || m_LastCellIndex != cellIndex) {

        int nextTargetIdx = m_Grid.GetNextCellId5Front(cellIndex, currentDirection, m_CurrentDirection);
        m_FollowTargetPath = m_Grid.FindPath(m_CurrentCellId, nextTargetIdx);
        m_FollowTargetPathIndex = 0;
        m_LastCellIndex = cellIndex;

        if (!m_FollowTargetPath.empty()) {
            m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
            std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
            m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
        }
    }

    if (m_FollowTargetPathIndex < m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
        std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (m_FollowTargetPathIndex >= m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_FollowTargetPath.clear();
    }
}

void PacManMoveComponent::FollowPathToPlayer(float deltaTime, int cellIndex)
{
    if (m_FollowTargetPath.empty() || m_LastCellIndex != cellIndex) {

        m_FollowTargetPath = m_Grid.FindPath(m_CurrentCellId, cellIndex);
        m_FollowTargetPathIndex = 0;
        m_LastCellIndex = cellIndex;

        if (!m_FollowTargetPath.empty()) {
            m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
            std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
            m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
        }
    }

    if (m_FollowTargetPathIndex < m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
        std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (m_FollowTargetPathIndex >= m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_FollowTargetPath.clear();
    }
}

void PacManMoveComponent::FleeFromPlayer(float deltaTime, int playerCellIndex) {
    if (m_FollowTargetPath.empty() || m_LastCellIndex != playerCellIndex) {
        m_FollowTargetPath = m_Grid.FindFarthestPath(m_CurrentCellId, playerCellIndex);
        m_FollowTargetPathIndex = 0;
        m_LastCellIndex = playerCellIndex;

        if (!m_FollowTargetPath.empty()) {
            m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
            std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
            m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
        }
    }

    if (m_FollowTargetPathIndex < m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_CurrentCellId = m_FollowTargetPath[m_FollowTargetPathIndex++];
        std::pair<float, float> targetCenter = m_Grid.GetCellCenter(m_CurrentCellId);
        m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
    }

    MoveEntity(deltaTime);

    if (m_FollowTargetPathIndex >= m_FollowTargetPath.size() && glm::length(m_pTransform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        m_FollowTargetPath.clear();
    }
}

void PacManMoveComponent::SetTargetCellIndex(int cellIndex)
{
    m_FollowTargetPath.clear();
    m_FollowTargetPathIndex = 0;
    m_FollowTargetPath = m_Grid.FindPath(m_CurrentCellId, cellIndex);
}

void PacManMoveComponent::MoveEntity(float deltaTime) {
    glm::vec3 currentPosition = m_pTransform->GetLocalPosition();
    glm::vec3 directionVec = glm::normalize(m_TargetPosition - currentPosition);
    glm::vec3 displacement = directionVec * m_Speed * deltaTime;

    if (glm::length(displacement) > glm::length(m_TargetPosition - currentPosition)) {
        displacement = m_TargetPosition - currentPosition;
    }

    if (m_TargetPosition == currentPosition) {
        displacement = glm::vec3(0.f, 0.f, 0.f);
    }

    m_pTransform->Translate(displacement);

    glm::vec2 direction2D = glm::normalize(glm::vec2(directionVec.x, directionVec.y));
    m_pTransform->SetDirection(direction2D);

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
    if (m_FollowTargetPathIndex >= m_FollowTargetPath.size())
        return true;

    glm::vec3 currentPosition = m_pTransform->GetLocalPosition();
    std::pair<float, float> nextCellCenter = m_Grid.GetCellCenter(m_FollowTargetPath[m_FollowTargetPathIndex]);
    glm::vec3 nextPosition = glm::vec3(nextCellCenter.first - 7.f, nextCellCenter.second - 8.f, 0.0f);

    float distance = glm::length(currentPosition - nextPosition);
    float threshold = 0.1f;

    return distance < threshold;
}

void PacManMoveComponent::SetFollowPath(bool followPath)
{
    m_FollowPath = followPath;
}

void PacManMoveComponent::SetSpeed(float speed)
{
    m_Speed = speed;
}

