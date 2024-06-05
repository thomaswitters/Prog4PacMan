#include "PacManMoveComponent.h"
using namespace dae;

PacManMoveComponent::PacManMoveComponent(std::weak_ptr<GameObject> pOwner, float speed) :
    BaseComponent(pOwner),
    m_CurrentDirection(glm::f32vec2(1.f, 0.f)),
    m_PreviusDirection(glm::f32vec2(1.f, 0.f)),
    m_Speed(speed),
    m_Transform(),
    m_Grid(10, 10, 50),
    m_CurentCellId(3),
    m_TargetPosition(glm::vec3(0.0f))
{
    m_Transform = pOwner.lock()->GetComponent<TransformComponent>();

    std::pair<float, float> initialCenter = m_Grid.getCellCenter(m_CurentCellId);

    glm::vec3 initialPosition = glm::vec3(initialCenter.first - 7.f, initialCenter.second - 8.f, 0.0f);
    m_TargetPosition = initialPosition;

    m_Grid.setCell(5, 5, CellType::Wall);
    m_Grid.setCell(3, 5, CellType::Wall);
    m_Grid.setCell(4, 5, CellType::Wall);

    m_Transform->SetLocalPosition(initialPosition);
}

void PacManMoveComponent::Update(float deltaTime)
{
    if (glm::length(m_Transform->GetLocalPosition() - m_TargetPosition) < 0.1f) {
        int nextCellId = m_Grid.getNextCellId(m_CurentCellId, m_CurrentDirection);

        std::pair<int, int> nextCellPosition = m_Grid.getCellPosition(nextCellId);
        if (nextCellPosition.first != -1 && nextCellPosition.second != -1) {
            CellType nextCellType = m_Grid.getCell(nextCellPosition.first, nextCellPosition.second);
            if (nextCellType == CellType::Tunnel) {
                m_CurentCellId = nextCellId;
                std::pair<float, float> targetCenter = m_Grid.getCellCenter(m_CurentCellId);
                m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);
            }
            else if (nextCellType == CellType::Wall)
            {

                nextCellId = m_Grid.getNextCellId(m_CurentCellId, m_PreviusDirection);
                m_CurentCellId = nextCellId;
                std::pair<float, float> targetCenter = m_Grid.getCellCenter(m_CurentCellId);
                m_TargetPosition = glm::vec3(targetCenter.first - 7.f, targetCenter.second - 8.f, 0.0f);       
            }
        }
    }

    glm::vec3 currentPosition = m_Transform->GetLocalPosition();
    glm::vec3 direction = glm::normalize(m_TargetPosition - currentPosition);
    glm::vec3 displacement = direction * m_Speed * deltaTime;

    if (glm::length(displacement) > glm::length(m_TargetPosition - currentPosition)) {
        displacement = m_TargetPosition - currentPosition;
    }

    m_Transform->Translate(displacement);
}

void PacManMoveComponent::Render() const
{
    m_Grid.drawGrid();
}

void PacManMoveComponent::SetDirection(Movement movement)
{
    glm::f32vec2 direction = glm::f32vec2(0.f, 0.f);

    int nextCellId = m_Grid.getNextCellId(m_CurentCellId, m_CurrentDirection);
    std::pair<int, int> nextCellPosition = m_Grid.getCellPosition(nextCellId);
    if (nextCellPosition.first != -1 && nextCellPosition.second != -1) {
        CellType nextCellType = m_Grid.getCell(nextCellPosition.first, nextCellPosition.second);
        if (nextCellType != CellType::Wall) {
            m_PreviusDirection = m_CurrentDirection;
            m_Transform->SetDirection(m_CurrentDirection);
        }
    }

    switch (movement) {
    case Movement::DOWN:
        m_CurrentDirection = glm::f32vec2{ 0.f, 1.f };
        break;
    case Movement::RIGHT:
        m_CurrentDirection = glm::f32vec2{ 1.f, 0.f };
        break;
    case Movement::UP:
        m_CurrentDirection = glm::f32vec2{ 0.f, -1.f };
        break;
    case Movement::LEFT:
        m_CurrentDirection = glm::f32vec2{ -1.f, 0.f };
        break;
    default:
        break;
    }
}

void PacManMoveComponent::SetAngle(float angle)
{
    m_Transform->SetAngle(angle);
}
