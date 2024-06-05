#include "Command.h"
#include "InputManager.h"


dae::MoveCommand::MoveCommand(std::shared_ptr<GameObject> object, float speed, glm::f32vec2 direction, bool useStickDir)
    : m_pObject{ object }
    , m_Speed{ speed }
    , m_Direction{ direction }
    , m_UseStickDir{ useStickDir }
{
}

void dae::MoveCommand::Execute(float deltaTime)
{
    auto objTransform{ m_pObject.lock()->GetComponent<TransformComponent>() };
    glm::vec3 currentPos{ objTransform->GetWorldPosition() };

    if (m_UseStickDir) {

        glm::vec2 stickValues = InputManager::GetInstance().GetControllerStickValues(GamePad::ControllerStick::LeftStick);

        float magnitude = glm::length(stickValues);
        if (magnitude > 1.0f) {
            stickValues /= magnitude;
        }

        m_Direction.x = stickValues.x;
        m_Direction.y = stickValues.y;
    }

    currentPos.x += m_Speed * m_Direction.x * deltaTime;
    currentPos.y += m_Speed * m_Direction.y * deltaTime;
    objTransform->SetLocalPosition(currentPos.x, currentPos.y, currentPos.z);

}

dae::ChangeMoveDirCommand::ChangeMoveDirCommand(std::shared_ptr<GameObject> object, glm::f32vec2 direction, float angle)
    : m_pObject{ object }
    , m_Direction{ direction }
    , m_Angle{ angle }
{
    m_pMoveComponent = m_pObject.lock()->GetComponent<PacManMoveComponent>();
}

void dae::ChangeMoveDirCommand::Execute(float)
{
    if (m_pMoveComponent != nullptr)
    {
        m_pMoveComponent->SetDirection(m_Direction);
        m_pMoveComponent->SetAngle(m_Angle);
    }
   
}


void dae::Command::Undo(float)
{

}

dae::AddPointsCommand::AddPointsCommand(std::shared_ptr<GameObject> object, int amount)
    : m_pObject{ object }
    , AmountPoints{ amount }
{
    m_pPoints = m_pObject.lock()->GetComponent<PointsComponent>();
}

void dae::AddPointsCommand::Execute(float)
{
    if (m_pPoints != nullptr)
    {
        m_pPoints.get()->AddPoints(AmountPoints);
    }
}

dae::RemoveHealthCommand::RemoveHealthCommand(std::shared_ptr<GameObject> object)
    : m_pObject{ object }
{
    m_pHealth = m_pObject.lock()->GetComponent<HealthComponent>();
}

void dae::RemoveHealthCommand::Execute(float)
{
    if (m_pHealth != nullptr)
    {
        m_pHealth.get()->RemoveHealth(1);
    }
}