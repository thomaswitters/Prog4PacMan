#include "Command.h"
#include "InputManager.h"
#include "PointsComponent.h"
#include "HealthComponent.h"

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

    // Check if useStickDir is true
    if (m_UseStickDir) {

        glm::vec2 stickValues = InputManager::GetInstance().GetControllerStickValues(GamePad::ControllerStick::LeftStick);

        // Normalize stickValues to ensure its magnitude is capped at 1
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


void dae::Command::Undo(float)
{

}

dae::AddPointsCommand::AddPointsCommand(std::shared_ptr<GameObject> object, int amount)
    : m_pObject{ object }
    , AmountPoints{ amount }
{
}

void dae::AddPointsCommand::Execute(float)
{
    auto points = m_pObject.lock().get()->GetComponent<PointsComponent>();
    points.get()->AddPoints(AmountPoints);
}

dae::RemoveHealthCommand::RemoveHealthCommand(std::shared_ptr<GameObject> object)
    : m_pObject{ object }
{
}

void dae::RemoveHealthCommand::Execute(float)
{
    auto Health = m_pObject.lock().get()->GetComponent<HealthComponent>();
    Health.get()->RemoveHealth(1);
}