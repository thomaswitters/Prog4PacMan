#include "VersusGameMode.h"

using namespace dae;

void VersusGameMode::SetupGameMode()
{
	m_Scene.RemoveAll();
	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "PacManLevel1.png");
	go->AddComponent(transform);
	transform->SetLocalPosition(100, 20, 0);
	go->AddComponent(RenderTexture);
	m_Scene.Add(go);

	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	transform = std::make_shared<dae::TransformComponent>(FPSCounter);
	FPSCounter->AddComponent(transform);
	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	FPSCounter->AddComponent(RenderTextFps);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	FPSCounter->AddComponent(TextFps);
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);
	FPSCounter->AddComponent(FPS);

	m_Scene.Add(FPSCounter);

	m_PacMan = std::make_shared<Peetje>(m_Scene, "pacman.png", 0, 3, 14, 8);
	m_PacMan->RespawnScoreAndHealth();
	m_PacMan->RespawnPacMan();

	auto& input = dae::InputManager::GetInstance();
	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	//input.SetKeyboardCommand(SDL_SCANCODE_KP_TAB, new dae::SwitchBetweenGameModesCommand(pacMan.GetPacMan()), dae::KeyState::keyDown);
	//input.SetKeyboardCommand(SDL_SCANCODE_RETURN, new dae::StartGame(pacMan.GetPacMan()), dae::KeyState::keyDown);


	std::string filePath = "../data/vertices.json";
	std::vector<glm::vec3> coinPositions = LoadPositionsFromJSON(filePath, "coins");


	for (size_t i = 0; i < coinPositions.size(); ++i)
	{
		auto coin = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(coin);
		coin->AddComponent(transform);
		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coinSmal.png");
		coin->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(coin, 5.f, 5.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		coin->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, dae::CollectableInfo(dae::Object::COIN, 10));
		coin->AddComponent(collectableComponent);
		collectableComponent->SetTotalCoins(int(coinPositions.size()));



		transform->SetLocalPosition(coinPositions[i]);

		m_Scene.Add(coin);
	}

	std::vector<glm::vec3> powerUpPositions = LoadPositionsFromJSON(filePath, "powerUps");

	for (size_t i = 0; i < powerUpPositions.size(); ++i)
	{
		auto PowerUp = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(PowerUp);
		PowerUp->AddComponent(transform);

		auto renderComponent = std::make_shared<dae::RenderComponent>(PowerUp, "PowerUp.png");
		PowerUp->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(PowerUp, 7.f, 7.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		PowerUp->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(PowerUp, dae::CollectableInfo(dae::Object::POWERUP, 50));
		PowerUp->AddComponent(collectableComponent);

		transform->SetLocalPosition(powerUpPositions[i]);

		m_Scene.Add(PowerUp);
	}

	auto Ghost1 = std::make_shared<dae::GameObject>();
	Ghost1->SetTag("Ghost");

	auto transform1 = std::make_shared<dae::TransformComponent>(Ghost1);
	Ghost1->AddComponent(transform1);
	auto PointComponentGhost1 = std::make_shared<dae::PointsComponent>(Ghost1);
	Ghost1->AddComponent(PointComponentGhost1);
	auto HealthComponentGhost1 = std::make_shared<dae::HealthComponent>(Ghost1);
	Ghost1->AddComponent(HealthComponentGhost1);
	auto BoxColliderComponentGhost1 = std::make_shared<dae::BoxColliderComponent>(Ghost1, 10.f, 10.f, glm::vec2(-7, -8));
	Ghost1->AddComponent(BoxColliderComponentGhost1);
	auto GhostcollectableComponent1 = std::make_shared<dae::CollectableComponent>(Ghost1, dae::CollectableInfo(dae::Object::GHOST, 1));
	Ghost1->AddComponent(GhostcollectableComponent1);
	auto fovComponent1 = std::make_shared<dae::FOVComponent>(Ghost1, 360.0f, 1000.f);
	Ghost1->AddComponent(fovComponent1);
	auto StateMangerMovement1 = std::make_shared<StateManagerMovement>(20.f, 7.f, 1.f);
	auto MoveComponent1 = std::make_shared<PacManMoveComponent>(Ghost1, 90.f, 8, 8, StateMangerMovement1);
	Ghost1->AddComponent(MoveComponent1);
	auto RenderGhost1 = std::make_shared<dae::RenderComponent>(Ghost1, "ghost.png");
	Ghost1->AddComponent(RenderGhost1);

	auto returnToBaseState1 = new FSMStates::ReturnToBase(Ghost1);
	auto noneFrightenedState1 = new FSMStates::NoneFrightened(Ghost1);
	auto noneState1 = new FSMStates::None(Ghost1);

	auto StateMachine1 = std::make_shared<dae::FiniteStateMachine>(Ghost1, returnToBaseState1);

	auto playerNotAtBaseCondition1 = new FSMConditions::NotAtBase(Ghost1);
	auto playerAtBaseCondition1 = new FSMConditions::AtBase(Ghost1);
	auto playerPoweredUpCondition1 = new FSMConditions::PlayerPoweredUp(Ghost1);
	auto playerNotPoweredUpCondition1 = new FSMConditions::PlayerNotPoweredUp(Ghost1);

	StateMachine1->AddTransition(noneState1, noneFrightenedState1, playerPoweredUpCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, noneState1, playerNotPoweredUpCondition1);
	StateMachine1->AddTransition(noneState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(returnToBaseState1, noneState1, playerAtBaseCondition1);

	Ghost1->AddComponent(StateMachine1);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	m_Scene.Add(Ghost1);

	auto Ghost2 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost2, "ghost2.png", std::vector<int>{322, 315, 250, 305}, 5.f, 20.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
	m_Scene.Add(Ghost2);

	auto Ghost3 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost3, "ghost3.png", std::vector<int>{0, 34, 37, 3}, 10.f, 20.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost3);

	auto Ghost4 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost4, "ghost4.png", std::vector<int>{306, 313, 242, 289}, 15.f, 20.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost4);
}
void VersusGameMode::SetupLevel2()
{
	m_Scene.RemoveAll();
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();
	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "PacManLevel2.png");
	go->AddComponent(transform);
	transform->SetLocalPosition(100, 20, 0);
	go->AddComponent(RenderTexture);
	m_Scene.Add(go);

	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	transform = std::make_shared<dae::TransformComponent>(FPSCounter);
	FPSCounter->AddComponent(transform);
	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	FPSCounter->AddComponent(RenderTextFps);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	FPSCounter->AddComponent(TextFps);
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);
	FPSCounter->AddComponent(FPS);

	m_Scene.Add(FPSCounter);

	m_PacMan->RespawnScoreAndHealth();
	m_PacMan->RespawnPacMan();

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);


	std::string filePath = "../data/vertices.json";
	std::vector<glm::vec3> coinPositions = LoadPositionsFromJSON(filePath, "coins");


	for (size_t i = 0; i < coinPositions.size(); ++i)
	{
		auto coin = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(coin);
		coin->AddComponent(transform);
		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coinSmal.png");
		coin->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(coin, 5.f, 5.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		coin->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, dae::CollectableInfo(dae::Object::COIN, 10));
		coin->AddComponent(collectableComponent);
		collectableComponent->SetTotalCoins(int(coinPositions.size()));



		transform->SetLocalPosition(coinPositions[i]);

		m_Scene.Add(coin);
	}

	std::vector<glm::vec3> powerUpPositions = LoadPositionsFromJSON(filePath, "powerUps");

	for (size_t i = 0; i < powerUpPositions.size(); ++i)
	{
		auto PowerUp = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(PowerUp);
		PowerUp->AddComponent(transform);

		auto renderComponent = std::make_shared<dae::RenderComponent>(PowerUp, "PowerUp.png");
		PowerUp->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(PowerUp, 7.f, 7.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		PowerUp->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(PowerUp, dae::CollectableInfo(dae::Object::POWERUP, 50));
		PowerUp->AddComponent(collectableComponent);

		transform->SetLocalPosition(powerUpPositions[i]);

		m_Scene.Add(PowerUp);
	}

	auto Ghost1 = std::make_shared<dae::GameObject>();
	Ghost1->SetTag("Ghost");

	auto transform1 = std::make_shared<dae::TransformComponent>(Ghost1);
	Ghost1->AddComponent(transform1);
	auto PointComponentGhost1 = std::make_shared<dae::PointsComponent>(Ghost1);
	Ghost1->AddComponent(PointComponentGhost1);
	auto HealthComponentGhost1 = std::make_shared<dae::HealthComponent>(Ghost1);
	Ghost1->AddComponent(HealthComponentGhost1);
	auto BoxColliderComponentGhost1 = std::make_shared<dae::BoxColliderComponent>(Ghost1, 10.f, 10.f, glm::vec2(-7, -8));
	Ghost1->AddComponent(BoxColliderComponentGhost1);
	auto GhostcollectableComponent1 = std::make_shared<dae::CollectableComponent>(Ghost1, dae::CollectableInfo(dae::Object::GHOST, 1));
	Ghost1->AddComponent(GhostcollectableComponent1);
	auto fovComponent1 = std::make_shared<dae::FOVComponent>(Ghost1, 360.0f, 1000.f);
	Ghost1->AddComponent(fovComponent1);
	auto StateMangerMovement1 = std::make_shared<StateManagerMovement>(20.f, 7.f, 1.f);
	auto MoveComponent1 = std::make_shared<PacManMoveComponent>(Ghost1, 90.f, 8, 8, StateMangerMovement1);
	Ghost1->AddComponent(MoveComponent1);
	auto RenderGhost1 = std::make_shared<dae::RenderComponent>(Ghost1, "ghost.png");
	Ghost1->AddComponent(RenderGhost1);

	auto returnToBaseState1 = new FSMStates::ReturnToBase(Ghost1);
	auto noneFrightenedState1 = new FSMStates::NoneFrightened(Ghost1);
	auto noneState1 = new FSMStates::None(Ghost1);

	auto StateMachine1 = std::make_shared<dae::FiniteStateMachine>(Ghost1, returnToBaseState1);

	auto playerNotAtBaseCondition1 = new FSMConditions::NotAtBase(Ghost1);
	auto playerAtBaseCondition1 = new FSMConditions::AtBase(Ghost1);
	auto playerPoweredUpCondition1 = new FSMConditions::PlayerPoweredUp(Ghost1);
	auto playerNotPoweredUpCondition1 = new FSMConditions::PlayerNotPoweredUp(Ghost1);

	StateMachine1->AddTransition(noneState1, noneFrightenedState1, playerPoweredUpCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, noneState1, playerNotPoweredUpCondition1);
	StateMachine1->AddTransition(noneState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(returnToBaseState1, noneState1, playerAtBaseCondition1);

	Ghost1->AddComponent(StateMachine1);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	m_Scene.Add(Ghost1);

	auto Ghost2 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost2, "ghost2.png", std::vector<int>{322, 315, 250, 305}, 5.f, 25.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
	m_Scene.Add(Ghost2);

	auto Ghost3 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost3, "ghost3.png", std::vector<int>{0, 34, 37, 3}, 10.f, 25.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost3);

	auto Ghost4 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost4, "ghost4.png", std::vector<int>{306, 313, 242, 289}, 15.f, 25.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost4);
}
void VersusGameMode::SetupLevel3()
{
	m_Scene.RemoveAll();
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();
	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "PacManLevel3.png");
	go->AddComponent(transform);
	transform->SetLocalPosition(100, 20, 0);
	go->AddComponent(RenderTexture);
	m_Scene.Add(go);

	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	transform = std::make_shared<dae::TransformComponent>(FPSCounter);
	FPSCounter->AddComponent(transform);
	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	FPSCounter->AddComponent(RenderTextFps);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	FPSCounter->AddComponent(TextFps);
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);
	FPSCounter->AddComponent(FPS);

	m_Scene.Add(FPSCounter);

	m_PacMan->RespawnScoreAndHealth();
	m_PacMan->RespawnPacMan();

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);


	std::string filePath = "../data/vertices.json";
	std::vector<glm::vec3> coinPositions = LoadPositionsFromJSON(filePath, "coins");


	for (size_t i = 0; i < coinPositions.size(); ++i)
	{
		auto coin = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(coin);
		coin->AddComponent(transform);
		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coinSmal.png");
		coin->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(coin, 5.f, 5.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		coin->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, dae::CollectableInfo(dae::Object::COIN, 10));
		coin->AddComponent(collectableComponent);
		collectableComponent->SetTotalCoins(int(coinPositions.size()));



		transform->SetLocalPosition(coinPositions[i]);

		m_Scene.Add(coin);
	}

	std::vector<glm::vec3> powerUpPositions = LoadPositionsFromJSON(filePath, "powerUps");

	for (size_t i = 0; i < powerUpPositions.size(); ++i)
	{
		auto PowerUp = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(PowerUp);
		PowerUp->AddComponent(transform);

		auto renderComponent = std::make_shared<dae::RenderComponent>(PowerUp, "PowerUp.png");
		PowerUp->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(PowerUp, 7.f, 7.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		PowerUp->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(PowerUp, dae::CollectableInfo(dae::Object::POWERUP, 50));
		PowerUp->AddComponent(collectableComponent);

		transform->SetLocalPosition(powerUpPositions[i]);

		m_Scene.Add(PowerUp);
	}

	auto Ghost1 = std::make_shared<dae::GameObject>();
	Ghost1->SetTag("Ghost");

	auto transform1 = std::make_shared<dae::TransformComponent>(Ghost1);
	Ghost1->AddComponent(transform1);
	auto PointComponentGhost1 = std::make_shared<dae::PointsComponent>(Ghost1);
	Ghost1->AddComponent(PointComponentGhost1);
	auto HealthComponentGhost1 = std::make_shared<dae::HealthComponent>(Ghost1);
	Ghost1->AddComponent(HealthComponentGhost1);
	auto BoxColliderComponentGhost1 = std::make_shared<dae::BoxColliderComponent>(Ghost1, 10.f, 10.f, glm::vec2(-7, -8));
	Ghost1->AddComponent(BoxColliderComponentGhost1);
	auto GhostcollectableComponent1 = std::make_shared<dae::CollectableComponent>(Ghost1, dae::CollectableInfo(dae::Object::GHOST, 1));
	Ghost1->AddComponent(GhostcollectableComponent1);
	auto fovComponent1 = std::make_shared<dae::FOVComponent>(Ghost1, 360.0f, 1000.f);
	Ghost1->AddComponent(fovComponent1);
	auto StateMangerMovement1 = std::make_shared<StateManagerMovement>(20.f, 7.f, 1.f);
	auto MoveComponent1 = std::make_shared<PacManMoveComponent>(Ghost1, 90.f, 8, 8, StateMangerMovement1);
	Ghost1->AddComponent(MoveComponent1);
	auto RenderGhost1 = std::make_shared<dae::RenderComponent>(Ghost1, "ghost.png");
	Ghost1->AddComponent(RenderGhost1);

	auto returnToBaseState1 = new FSMStates::ReturnToBase(Ghost1);
	auto noneFrightenedState1 = new FSMStates::NoneFrightened(Ghost1);
	auto noneState1 = new FSMStates::None(Ghost1);

	auto StateMachine1 = std::make_shared<dae::FiniteStateMachine>(Ghost1, returnToBaseState1);

	auto playerNotAtBaseCondition1 = new FSMConditions::NotAtBase(Ghost1);
	auto playerAtBaseCondition1 = new FSMConditions::AtBase(Ghost1);
	auto playerPoweredUpCondition1 = new FSMConditions::PlayerPoweredUp(Ghost1);
	auto playerNotPoweredUpCondition1 = new FSMConditions::PlayerNotPoweredUp(Ghost1);

	StateMachine1->AddTransition(noneState1, noneFrightenedState1, playerPoweredUpCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, noneState1, playerNotPoweredUpCondition1);
	StateMachine1->AddTransition(noneState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(noneFrightenedState1, returnToBaseState1, playerNotAtBaseCondition1);
	StateMachine1->AddTransition(returnToBaseState1, noneState1, playerAtBaseCondition1);

	Ghost1->AddComponent(StateMachine1);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	m_Scene.Add(Ghost1);

	auto Ghost2 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost2, "ghost2.png", std::vector<int>{322, 315, 250, 305}, 5.f, 30.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
	m_Scene.Add(Ghost2);

	auto Ghost3 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost3, "ghost3.png", std::vector<int>{0, 34, 37, 3}, 10.f, 30.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost3);

	auto Ghost4 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost4, "ghost4.png", std::vector<int>{306, 313, 242, 289}, 15.f, 30.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost4);
}

void VersusGameMode::InitializeGhost(std::shared_ptr<dae::GameObject> ghost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, float maxTimeInChase, FSMStates::ChasePlayer::FindPathType pathType) {
	ghost->SetTag("Ghost");

	auto transform = std::make_shared<dae::TransformComponent>(ghost);
	ghost->AddComponent(transform);

	auto renderComponent = std::make_shared<dae::RenderComponent>(ghost, texturePath);
	ghost->AddComponent(renderComponent);

	auto boxColliderComponent = std::make_shared<dae::BoxColliderComponent>(ghost, 10.f, 10.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
	ghost->AddComponent(boxColliderComponent);

	auto collectableComponent = std::make_shared<dae::CollectableComponent>(ghost, dae::CollectableInfo(dae::Object::GHOST, 1));
	ghost->AddComponent(collectableComponent);

	auto fovComponent = std::make_shared<dae::FOVComponent>(ghost, 360.0f, 1000.f);
	ghost->AddComponent(fovComponent);

	auto stateManagerMovement = std::make_shared<StateManagerMovement>(maxTimeInChase, 7.f, maxTimeInBase);
	auto moveComponent = std::make_shared<PacManMoveComponent>(ghost, 90.f, 8, 8, stateManagerMovement);
	ghost->AddComponent(moveComponent);

	auto patrolState = new FSMStates::Patrol(ghost, patrolPoints);
	auto chaseState = new FSMStates::ChasePlayer(ghost, pathType);
	auto returnToBaseState = new FSMStates::ReturnToBase(ghost);
	auto frightenedState = new FSMStates::Frightened(ghost);

	auto stateMachine = std::make_shared<dae::FiniteStateMachine>(ghost, returnToBaseState);

	auto playerNotAtBaseCondition = new FSMConditions::NotAtBase(ghost);
	auto playerAtBaseCondition = new FSMConditions::AtBase(ghost);
	auto playerTimeInChaseStateCondition = new FSMConditions::PlayerTimerChase(ghost);
	auto playerTimeInPatrolStateCondition = new FSMConditions::PlayerTimerPatrol(ghost);
	auto playerPoweredUpCondition = new FSMConditions::PlayerPoweredUp(ghost);
	auto playerNotPoweredUpCondition = new FSMConditions::PlayerNotPoweredUp(ghost);

	stateMachine->AddTransition(chaseState, frightenedState, playerPoweredUpCondition);
	stateMachine->AddTransition(patrolState, frightenedState, playerPoweredUpCondition);
	stateMachine->AddTransition(frightenedState, chaseState, playerNotPoweredUpCondition);
	stateMachine->AddTransition(chaseState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(patrolState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(frightenedState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(returnToBaseState, patrolState, playerAtBaseCondition);
	stateMachine->AddTransition(patrolState, chaseState, playerTimeInPatrolStateCondition);
	stateMachine->AddTransition(chaseState, patrolState, playerTimeInChaseStateCondition);

	ghost->AddComponent(stateMachine);
}

std::vector<glm::vec3> VersusGameMode::LoadPositionsFromJSON(const std::string& filePath, const std::string& type) {
	std::vector<glm::vec3> positions;

	std::ifstream file(filePath);
	if (file.is_open()) {
		try {
			json jsonData;
			file >> jsonData;

			if (jsonData.find(type) != jsonData.end()) {
				for (const auto& position : jsonData[type]) {
					positions.emplace_back(glm::vec3(position[0], position[1], position[2]));
				}
			}
			else {
				throw std::runtime_error("Error: Could not find \"" + type + "\" in JSON data.");
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error while parsing JSON from file " << filePath << ": " << ex.what() << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Error: Could not open file " << filePath << std::endl;
	}

	return positions;
}