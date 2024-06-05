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

dae::ChangeMoveDirCommand::ChangeMoveDirCommand(std::shared_ptr<GameObject> object, PacManMoveComponent::Movement movement, float angle)
    : m_pObject{ object }
    , m_Movement{ movement }
    , m_Angle{ angle }
{
    m_pMoveComponent = m_pObject.lock()->GetComponent<PacManMoveComponent>();
    m_pTransformComponent = m_pObject.lock()->GetComponent<TransformComponent>();
}

void dae::ChangeMoveDirCommand::Execute(float)
{
    if (m_pMoveComponent != nullptr)
    {
        m_pMoveComponent->SetDirection(m_Movement);
    }
   
}


void dae::Command::Undo(float)
{

}

dae::AddPointsCommand::AddPointsCommand(std::shared_ptr<GameObject> object, int amount)
    : m_pObject{ object }
    , AmountPoints{ amount }
{
    m_pPoints = m_pObject->GetComponent<PointsComponent>();
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

int dae::SwitchBetweenGameModesCommand::m_CurrentLevelIndex;
dae::SwitchBetweenGameModesCommand::SwitchBetweenGameModesCommand(std::shared_ptr<GameObject> object)
    : m_pObject{ object }
    , m_StartString{}
{
    m_StartString = "Level1";
    m_pRenderComponent = object->GetComponent<RenderComponent>();
}

void dae::SwitchBetweenGameModesCommand::Execute(float)
{
    ++m_CurrentLevelIndex;

    switch (m_CurrentLevelIndex)
    {
    case 0:
        m_pRenderComponent->SetTexture("pacman.png");
        m_StartString = "Level1";
        break;
    case 1:
        m_pRenderComponent->SetTexture("msPacmanAndPacman.png");
        m_StartString = "Level2";
        break;
    case 2:
        m_pRenderComponent->SetTexture("GhostAndPacman.png");
        m_StartString = "Level3";
        break;
    default:
        m_pRenderComponent->SetTexture("pacman.png");
        m_CurrentLevelIndex = 0;
        m_StartString = "Level1";
        break;
    }

   
}

dae::StartGame::StartGame(std::vector<std::shared_ptr<dae::GameMode>> gameModes, SwitchBetweenGameModesCommand& switchCommand)
    : m_GameModes{ gameModes }
    , m_SwitchCommand(switchCommand)
    , m_StartString()
{

}

void dae::StartGame::Execute(float)
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    const std::string& currentSceneName = sceneManager.GetActiveScene().GetName();
    const std::string& newSceneName = m_SwitchCommand.GetStartLevel();
    
    if (currentSceneName != newSceneName) {
        sceneManager.SetActiveScene(newSceneName);

        if (newSceneName == "Level3")
        {
            m_GameModes[2]->SetupGameMode();
        }
        else if (newSceneName == "Level2")
        {
            m_GameModes[1]->SetupGameMode();
        }
        else if (newSceneName == "Level1")
        {
            m_GameModes[0]->SetupGameMode();
        }
    }
}