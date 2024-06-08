#include "VersusGameMode.h"

using namespace dae;

void VersusGameMode::SetupGameMode()
{
	m_Scene.RemoveAll();
	m_pPacMan = std::make_shared<Peetje>(m_Scene, "pacman.png", 0, 3, 14, 8, glm::vec2(510, 40), glm::vec2(510, 60));
	SetupLevel("PacManLevel1.png", 20.f);
}
void VersusGameMode::SetupLevel2()
{
	m_Scene.RemoveAll();
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();
	SetupLevel("PacManLevel2.png", 25.f);
}
void VersusGameMode::SetupLevel3()
{
	m_Scene.RemoveAll();
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();
	SetupLevel("PacManLevel3.png", 30.f);
}

void VersusGameMode::SetupHighScoreScreen()
{
	SetupHighscores();

	auto fontEndSceneWon = ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);

	auto EndScreenPeetje = std::make_shared<dae::GameObject>();

	auto transformEndScreenPeetje = std::make_shared<dae::TransformComponent>(EndScreenPeetje);
	auto RenderTextureEndScreenPeetje = std::make_shared<dae::RenderComponent>(EndScreenPeetje, "EndScreenPhoto.png");
	EndScreenPeetje->AddComponent(transformEndScreenPeetje);
	transformEndScreenPeetje->SetLocalPosition(30, 100, 0);
	EndScreenPeetje->AddComponent(RenderTextureEndScreenPeetje);
	m_Scene.Add(EndScreenPeetje);

	auto EndSceneWon = std::make_shared<GameObject>();
	auto transFormEndSceneWon = std::make_shared<TransformComponent>(EndSceneWon);
	EndSceneWon->AddComponent(transFormEndSceneWon);
	auto renderEndSceneWon = std::make_shared<RenderComponent>(EndSceneWon);
	EndSceneWon->AddComponent(renderEndSceneWon);
	auto textEndSceneWon = std::make_shared<TextComponent>(EndSceneWon, "Congratulations You won !!!", fontEndSceneWon, SDL_Color{ 255, 255, 0, 255 });
	EndSceneWon->AddComponent(textEndSceneWon);
	transFormEndSceneWon->SetLocalPosition(50, 45, 0);
	m_Scene.Add(EndSceneWon);

	auto PlayerScore = std::make_shared<GameObject>();
	auto transformComponent = std::make_shared<TransformComponent>(PlayerScore);
	PlayerScore->AddComponent(transformComponent);
	auto renderTextScore = std::make_shared<RenderComponent>(PlayerScore);
	PlayerScore->AddComponent(renderTextScore);
	auto TextScore = std::make_shared<TextComponent>(PlayerScore, "Your score is: " + std::to_string(m_Score), fontEndSceneWon, SDL_Color{ 255, 255, 255, 255 });
	PlayerScore->AddComponent(TextScore);
	transformComponent->SetLocalPosition(410, 45, 0);
	m_Scene.Add(PlayerScore);
}
void VersusGameMode::SetupLosHighScoreScreen()
{
	SetupHighscores();

	auto fontEndSceneWon = ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);

	auto EndScreenLostPeetje = std::make_shared<dae::GameObject>();

	auto transformEndScreenLostPeetje = std::make_shared<dae::TransformComponent>(EndScreenLostPeetje);
	auto RenderTextureEndScreenLostPeetje = std::make_shared<dae::RenderComponent>(EndScreenLostPeetje, "EndScreenGhost.png");
	EndScreenLostPeetje->AddComponent(transformEndScreenLostPeetje);
	transformEndScreenLostPeetje->SetLocalPosition(30, 100, 0);
	EndScreenLostPeetje->AddComponent(RenderTextureEndScreenLostPeetje);
	m_Scene.Add(EndScreenLostPeetje);

	auto EndSceneWon = std::make_shared<GameObject>();
	auto transFormEndSceneWon = std::make_shared<TransformComponent>(EndSceneWon);
	EndSceneWon->AddComponent(transFormEndSceneWon);
	auto renderEndSceneWon = std::make_shared<RenderComponent>(EndSceneWon);
	EndSceneWon->AddComponent(renderEndSceneWon);
	auto textEndSceneWon = std::make_shared<TextComponent>(EndSceneWon, "YOU LOST !", fontEndSceneWon, SDL_Color{ 255, 0, 0, 255 });
	EndSceneWon->AddComponent(textEndSceneWon);
	transFormEndSceneWon->SetLocalPosition(150, 45, 0);
	m_Scene.Add(EndSceneWon);

	auto PlayerScore = std::make_shared<GameObject>();
	auto transformComponent = std::make_shared<TransformComponent>(PlayerScore);
	PlayerScore->AddComponent(transformComponent);
	auto renderTextScore = std::make_shared<RenderComponent>(PlayerScore);
	PlayerScore->AddComponent(renderTextScore);
	auto TextScore = std::make_shared<TextComponent>(PlayerScore, "Your score is: " + std::to_string(m_Score), fontEndSceneWon, SDL_Color{ 255, 255, 255, 255 });
	PlayerScore->AddComponent(TextScore);
	transformComponent->SetLocalPosition(410, 45, 0);
	m_Scene.Add(PlayerScore);
}

void VersusGameMode::SetupHighscores()
{
	auto fontEndSceneWon = ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);
	m_Score = m_pPacMan->GetScore();

	auto& highscoreManager = dae::HighscoreManager::GetInstance();
	std::vector<dae::HighscoreManager::Score> highscores = highscoreManager.LoadVersusHighscores();
	highscores.push_back({ highscoreManager.GetPlayerName(), m_Score });
	std::sort(highscores.begin(), highscores.end(), dae::HighscoreManager::CompareScores);
	if (highscores.size() > 20) {
		highscores.resize(20);
	}

	highscoreManager.SaveVersusHighscores(highscores);

	m_Scene.RemoveAll();
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();

	auto BackGround = std::make_shared<dae::GameObject>();

	auto transformBackGround = std::make_shared<dae::TransformComponent>(BackGround);
	auto RenderTextureBackGround = std::make_shared<dae::RenderComponent>(BackGround, "background.tga");
	BackGround->AddComponent(transformBackGround);
	BackGround->AddComponent(RenderTextureBackGround);
	m_Scene.Add(BackGround);


	auto TitelHighscore = std::make_shared<GameObject>();
	auto transFormTitelHighscore = std::make_shared<TransformComponent>(TitelHighscore);
	TitelHighscore->AddComponent(transFormTitelHighscore);
	auto renderTitelHighscore = std::make_shared<RenderComponent>(TitelHighscore);
	TitelHighscore->AddComponent(renderTitelHighscore);
	auto textTitelHighscore = std::make_shared<TextComponent>(TitelHighscore, "TOP 5 HIGHSCORES", fontEndSceneWon, SDL_Color{ 243, 132, 41, 255 });
	TitelHighscore->AddComponent(textTitelHighscore);
	transFormTitelHighscore->SetLocalPosition(380, 100, 0);
	m_Scene.Add(TitelHighscore);

	float yPos = 130;
	size_t numHighscoresToShow = std::min(highscores.size(), static_cast<size_t>(5));
	for (size_t i = 0; i < numHighscoresToShow; ++i) {
		auto playerScore = std::make_shared<GameObject>();
		auto transformComponent = std::make_shared<TransformComponent>(playerScore);
		playerScore->AddComponent(transformComponent);
		auto renderTextScore = std::make_shared<RenderComponent>(playerScore);
		playerScore->AddComponent(renderTextScore);
		auto textScore = std::make_shared<TextComponent>(playerScore, highscores[i].playerName + ": " + std::to_string(highscores[i].score), fontEndSceneWon, SDL_Color{ 255, 255, 255, 255 });
		playerScore->AddComponent(textScore);
		transformComponent->SetLocalPosition(410, yPos, 0);
		m_Scene.Add(playerScore);
		yPos += 30;
	}
}


void VersusGameMode::SetupLevel(const std::string& levelTexture, float ghostChaseTime)
{
	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, levelTexture);
	go->AddComponent(transform);
	transform->SetLocalPosition(100, 20, 0);
	go->AddComponent(RenderTexture);
	m_Scene.Add(go);

	SetupFPSCounter();

	m_pPacMan->RespawnScoreAndHealth();
	m_pPacMan->RespawnPacMan();

	SetupInputCommands();

	std::string filePath = "../data/vertices.json";
	SetupCollectables(filePath, "powerUps", "PowerUp.png", 7.f, 7.f, dae::Object::POWERUP, 50);
	SetupCollectables(filePath, "coins", "coinSmal.png", 5.f, 5.f, dae::Object::COIN, 10);

	SetupGhosts(ghostChaseTime);
}
void VersusGameMode::SetupFPSCounter()
{
	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto transform = std::make_shared<dae::TransformComponent>(FPSCounter);
	FPSCounter->AddComponent(transform);
	transform->SetLocalPosition(10, 10, 0);

	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	FPSCounter->AddComponent(RenderTextFps);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	FPSCounter->AddComponent(TextFps);
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);
	FPSCounter->AddComponent(FPS);

	m_Scene.Add(FPSCounter);
}
void VersusGameMode::SetupInputCommands()
{
	auto& input = dae::InputManager::GetInstance();
	auto pacMan = m_pPacMan->GetPacMan();

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(pacMan, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(pacMan, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(pacMan, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(pacMan, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);
}
void VersusGameMode::SetupCollectables(const std::string& filePath, const std::string& key, const std::string& texture, float width, float height, dae::Object type, int value)
{
	std::vector<glm::vec3> positions = LoadPositionsFromJSON(filePath, key);

	for (const auto& pos : positions)
	{
		auto collectable = std::make_shared<dae::GameObject>();

		auto transform = std::make_shared<dae::TransformComponent>(collectable);
		collectable->AddComponent(transform);
		auto renderComponent = std::make_shared<dae::RenderComponent>(collectable, texture);
		collectable->AddComponent(renderComponent);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(collectable, width, height, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
		collectable->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(collectable, dae::CollectableInfo(type, value));
		collectable->AddComponent(collectableComponent);
		collectableComponent->SetTotalCoins(int(positions.size()));

		transform->SetLocalPosition(pos);

		m_Scene.Add(collectable);
	}
}
void VersusGameMode::SetupGhosts(float chaseTime)
{
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

	auto& input = dae::InputManager::GetInstance();
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::ChangeMoveDirCommand(Ghost1, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

	m_Scene.Add(Ghost1);

	auto Ghost2 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost2, "ghost2.png", std::vector<int>{322, 315, 250, 305}, 5.f, chaseTime, FSMStates::ChasePlayer::FindPathType::BESTPATH);
	m_Scene.Add(Ghost2);

	auto Ghost3 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost3, "ghost3.png", std::vector<int>{0, 34, 37, 3}, 10.f, chaseTime, FSMStates::ChasePlayer::FindPathType::PREDICT);
	m_Scene.Add(Ghost3);

	auto Ghost4 = std::make_shared<dae::GameObject>();
	InitializeGhost(Ghost4, "ghost4.png", std::vector<int>{306, 313, 242, 289}, 15.f, chaseTime, FSMStates::ChasePlayer::FindPathType::PREDICT);
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