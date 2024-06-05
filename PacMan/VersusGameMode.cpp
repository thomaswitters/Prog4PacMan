#include "VersusGameMode.h"

using namespace dae;

void VersusGameMode::SetupGameMode()
{
	m_Scene.RemoveAll();
	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "PacManSprites.png");
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

	Peetje pacMan(m_Scene, "pacman.png", 0, 3, 14, 8);

	auto& input = dae::InputManager::GetInstance();
	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(pacMan.GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(pacMan.GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(pacMan.GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(pacMan.GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

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

	{
		auto Ghost2 = std::make_shared<dae::GameObject>();
		Ghost2->SetTag("Ghost");

		auto transform2 = std::make_shared<dae::TransformComponent>(Ghost2);
		Ghost2->AddComponent(transform2);
		auto PointComponentGhost2 = std::make_shared<dae::PointsComponent>(Ghost2);
		Ghost2->AddComponent(PointComponentGhost2);
		auto HealthComponentGhost2 = std::make_shared<dae::HealthComponent>(Ghost2);
		Ghost2->AddComponent(HealthComponentGhost2);
		auto BoxColliderComponentGhost2 = std::make_shared<dae::BoxColliderComponent>(Ghost2, 10.f, 10.f, glm::vec2(-7, -8));
		Ghost2->AddComponent(BoxColliderComponentGhost2);
		auto GhostcollectableComponent2 = std::make_shared<dae::CollectableComponent>(Ghost2, dae::CollectableInfo(dae::Object::GHOST, 1));
		Ghost2->AddComponent(GhostcollectableComponent2);
		auto fovComponent2 = std::make_shared<dae::FOVComponent>(Ghost2, 360.0f, 1000.f);
		Ghost2->AddComponent(fovComponent2);
		auto StateMangerMovement1 = std::make_shared<StateManagerMovement>(20.f, 7.f, 1.f);
		auto MoveComponent2 = std::make_shared<PacManMoveComponent>(Ghost2, 90.f, 8, 8, StateMangerMovement1);
		Ghost2->AddComponent(MoveComponent2);
		auto RenderGhost2 = std::make_shared<dae::RenderComponent>(Ghost2, "ghost.png");
		Ghost2->AddComponent(RenderGhost2);

		auto returnToBaseState2 = new FSMStates::ReturnToBase(Ghost2);
		auto noneFrightenedState2 = new FSMStates::NoneFrightened(Ghost2);
		auto noneState2 = new FSMStates::None(Ghost2);

		auto StateMachine2 = std::make_shared<dae::FiniteStateMachine>(Ghost2, returnToBaseState2);

		auto playerNotAtBaseCondition2 = new FSMConditions::NotAtBase(Ghost2);
		auto playerAtBaseCondition2 = new FSMConditions::AtBase(Ghost2);
		auto playerPoweredUpCondition2 = new FSMConditions::PlayerPoweredUp(Ghost2);
		auto playerNotPoweredUpCondition2 = new FSMConditions::PlayerNotPoweredUp(Ghost2);

		StateMachine2->AddTransition(noneState2, noneFrightenedState2, playerPoweredUpCondition2);
		StateMachine2->AddTransition(noneFrightenedState2, noneState2, playerNotPoweredUpCondition2);
		StateMachine2->AddTransition(noneState2, returnToBaseState2, playerNotAtBaseCondition2);
		StateMachine2->AddTransition(noneFrightenedState2, returnToBaseState2, playerNotAtBaseCondition2);
		StateMachine2->AddTransition(returnToBaseState2, noneState2, playerAtBaseCondition2);


		Ghost2->AddComponent(StateMachine2);

		input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(Ghost2, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
		input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(Ghost2, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
		input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(Ghost2, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
		input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(Ghost2, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

		//input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonA, new dae::StartGame(Ghost2), dae::KeyState::keyDown);

		m_Scene.Add(Ghost2);
	}

	auto Ghost2 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost2, "ghost2.png", std::vector<int>{322, 315, 250, 305}, 5.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
	m_Scene.Add(Ghost2);

	auto Ghost3 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost3, "ghost3.png", std::vector<int>{0, 34, 37, 3}, 10.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost3);

	auto Ghost4 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost4, "ghost4.png", std::vector<int>{306, 313, 242, 289}, 15.f, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost4);
}

void VersusGameMode::InitializeGhost(std::shared_ptr<dae::GameObject> ghost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, FSMStates::ChasePlayer::FindPathType pathType) {
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

	auto stateManagerMovement = std::make_shared<StateManagerMovement>(20.f, 7.f, maxTimeInBase);
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