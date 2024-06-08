#include "SoloGameMode.h"
using namespace dae;

void SoloGameMode::SetupGameMode()
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
	transform->SetLocalPosition(10, 10, 0);

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
	//m_Scene.Add(m_PacMan->GetPacMan());

	auto& input = dae::InputManager::GetInstance();
	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);


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
	InitializeGhost(Ghost1, "ghost.png", std::vector<int>{16, 9, 43, 49}, 1.f, 20.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
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

void SoloGameMode::SetupLevel2()
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

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

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
	InitializeGhost(Ghost1, "ghost.png", std::vector<int>{16, 9, 43, 49}, 1.f, 25.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
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

void SoloGameMode::SetupLevel3()
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

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(m_PacMan->GetPacMan(), PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);


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
	InitializeGhost(Ghost1, "ghost.png", std::vector<int>{16, 9, 43, 49}, 1.f, 30.f, FSMStates::ChasePlayer::FindPathType::BESTPATH);
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

void SoloGameMode::InitializeGhost(std::shared_ptr<dae::GameObject> pGhost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, float maxTimeInChase ,FSMStates::ChasePlayer::FindPathType pathType) {
	pGhost->SetTag("Ghost");

	auto transform = std::make_shared<dae::TransformComponent>(pGhost);
	pGhost->AddComponent(transform);

	auto renderComponent = std::make_shared<dae::RenderComponent>(pGhost, texturePath);
	pGhost->AddComponent(renderComponent);

	auto boxColliderComponent = std::make_shared<dae::BoxColliderComponent>(pGhost, 10.f, 10.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
	pGhost->AddComponent(boxColliderComponent);

	auto collectableComponent = std::make_shared<dae::CollectableComponent>(pGhost, dae::CollectableInfo(dae::Object::GHOST, 1));
	pGhost->AddComponent(collectableComponent);

	auto fovComponent = std::make_shared<dae::FOVComponent>(pGhost, 360.0f, 1000.f);
	pGhost->AddComponent(fovComponent);

	auto stateManagerMovement = std::make_shared<StateManagerMovement>(maxTimeInChase, 7.f, maxTimeInBase);
	auto moveComponent = std::make_shared<PacManMoveComponent>(pGhost, 90.f, 8, 8, stateManagerMovement);
	pGhost->AddComponent(moveComponent);

	auto patrolState = new FSMStates::Patrol(pGhost, patrolPoints);
	auto chaseState = new FSMStates::ChasePlayer(pGhost, pathType);
	auto returnToBaseState = new FSMStates::ReturnToBase(pGhost);
	auto frightenedState = new FSMStates::Frightened(pGhost);

	auto stateMachine = std::make_shared<dae::FiniteStateMachine>(pGhost, returnToBaseState);

	auto playerNotAtBaseCondition = new FSMConditions::NotAtBase(pGhost);
	auto playerAtBaseCondition = new FSMConditions::AtBase(pGhost);
	auto playerTimeInChaseStateCondition = new FSMConditions::PlayerTimerChase(pGhost);
	auto playerTimeInPatrolStateCondition = new FSMConditions::PlayerTimerPatrol(pGhost);
	auto playerPoweredUpCondition = new FSMConditions::PlayerPoweredUp(pGhost);
	auto playerNotPoweredUpCondition = new FSMConditions::PlayerNotPoweredUp(pGhost);

	stateMachine->AddTransition(chaseState, frightenedState, playerPoweredUpCondition);
	stateMachine->AddTransition(patrolState, frightenedState, playerPoweredUpCondition);
	stateMachine->AddTransition(frightenedState, patrolState, playerNotPoweredUpCondition);
	stateMachine->AddTransition(chaseState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(patrolState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(frightenedState, returnToBaseState, playerNotAtBaseCondition);
	stateMachine->AddTransition(returnToBaseState, patrolState, playerAtBaseCondition);
	stateMachine->AddTransition(patrolState, chaseState, playerTimeInPatrolStateCondition);
	stateMachine->AddTransition(chaseState, patrolState, playerTimeInChaseStateCondition);

	pGhost->AddComponent(stateMachine);
}

std::vector<glm::vec3> SoloGameMode::LoadPositionsFromJSON(const std::string& filePath, const std::string& type) {
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