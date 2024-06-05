// PacMan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "RotationComponent.h"
#include "InputManager.h"
#include "ScoreObserver.h"
#include "PointsComponent.h"
#include "HealthObserver.h"
#include "HealthComponent.h"
#include <thread>
#include "CollectableComponent.h"
#include "BoxColliderComponent.h"
#include <FiniteStateMachineComponent.h>
#include <GhosStatesAndTransitions.h>
#include <FOVComponent.h>

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "PacManSprites.png");
	go->AddComponent(transform);
	transform->SetLocalPosition(100, 20, 0);
	go->AddComponent(RenderTexture);
	scene.Add(go);

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

	scene.Add(FPSCounter);


	auto PlayerPacManScore = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerPacManScore);
	PlayerPacManScore->AddComponent(transform);
	auto RenderTextPacManScore = std::make_shared<dae::RenderComponent>(PlayerPacManScore);
	PlayerPacManScore->AddComponent(RenderTextPacManScore);
	std::shared_ptr<dae::TextComponent> textPacManScore = std::make_shared<dae::TextComponent>(PlayerPacManScore, "Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerPacManScore->AddComponent(textPacManScore);
	transform->SetLocalPosition(0, 165, 0);
	scene.Add(PlayerPacManScore);

	auto PlayerPacManHealth = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerPacManHealth);
	PlayerPacManHealth->AddComponent(transform);
	auto RenderTextPlayerHealth = std::make_shared<dae::RenderComponent>(PlayerPacManHealth);
	PlayerPacManHealth->AddComponent(RenderTextPlayerHealth);
	std::shared_ptr<dae::TextComponent> textPlayerHealth = std::make_shared<dae::TextComponent>(PlayerPacManHealth, "# lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerPacManHealth->AddComponent(textPlayerHealth);
	transform->SetLocalPosition(0, 150, 0);
	scene.Add(PlayerPacManHealth);

	auto ScoreObserverPacMan = std::make_shared<dae::ScoreObserver>(PlayerPacManScore);
	auto HealthObserverPacMan = std::make_shared<dae::HealthObserver>(PlayerPacManHealth);

	auto PacMan = std::make_shared<dae::GameObject>();
	PacMan->SetTag("Player");

	transform = std::make_shared<dae::TransformComponent>(PacMan);
	PacMan->AddComponent(transform);
	auto MovePacManInDir = std::make_shared<dae::PacManMoveComponent>(PacMan, 60.f);
	PacMan->AddComponent(MovePacManInDir);
	auto PointComponentPacMan = std::make_shared<dae::PointsComponent>(PacMan);
	PacMan->AddComponent(PointComponentPacMan);
	auto HealthComponentPacMan = std::make_shared<dae::HealthComponent>(PacMan);
	PacMan->AddComponent(HealthComponentPacMan);
	auto BoxColliderComponentPacMan = std::make_shared<dae::BoxColliderComponent>(PacMan, 13.f, 13.f, glm::vec2(-8, -8));
	PacMan->AddComponent(BoxColliderComponentPacMan);
	auto RenderPacMan = std::make_shared<dae::RenderComponent>(PacMan, "pacman.png");
	PacMan->AddComponent(RenderPacMan);

	PointComponentPacMan->GetPointSubject()->AddObserver(ScoreObserverPacMan);
	HealthComponentPacMan->GetHealthSubject()->AddObserver(HealthObserverPacMan);


	//BoxColliderComponentPacMan->SetTrigger(false);
	transform->SetLocalPosition(100, 200, 0);

	scene.Add(PacMan);



	auto PlayerGhostScore = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerGhostScore);
	PlayerGhostScore->AddComponent(transform);
	auto RenderTextGhostScore = std::make_shared<dae::RenderComponent>(PlayerGhostScore);
	PlayerGhostScore->AddComponent(RenderTextGhostScore);
	std::shared_ptr<dae::TextComponent> textGhostScore = std::make_shared<dae::TextComponent>(PlayerGhostScore, "Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerGhostScore->AddComponent(textGhostScore);
	transform->SetLocalPosition(0, 215, 0);
	scene.Add(PlayerGhostScore);

	auto PlayerGhostHealth = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerGhostHealth);
	PlayerGhostHealth->AddComponent(transform);
	auto RenderTextGhostHealth = std::make_shared<dae::RenderComponent>(PlayerGhostHealth);
	PlayerGhostHealth->AddComponent(RenderTextGhostHealth);
	std::shared_ptr<dae::TextComponent> textGhostHealth = std::make_shared<dae::TextComponent>(PlayerGhostHealth, "# lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerGhostHealth->AddComponent(textGhostHealth);
	transform->SetLocalPosition(0, 200, 0);
	scene.Add(PlayerGhostHealth);

	auto ScoreObserverGhost = std::make_shared<dae::ScoreObserver>(PlayerGhostScore);
	auto HealthObserverGhost = std::make_shared<dae::HealthObserver>(PlayerGhostHealth);

	auto Ghost = std::make_shared<dae::GameObject>();
	Ghost->SetTag("Ghost");

	transform = std::make_shared<dae::TransformComponent>(Ghost);
	//transform->Rotate(90.f);
	Ghost->AddComponent(transform);
	auto PointComponentGhost = std::make_shared<dae::PointsComponent>(Ghost);
	Ghost->AddComponent(PointComponentGhost);
	auto HealthComponentGhost = std::make_shared<dae::HealthComponent>(Ghost);
	Ghost->AddComponent(HealthComponentGhost);
	auto BoxColliderComponentGhost = std::make_shared<dae::BoxColliderComponent>(Ghost, 20.f, 20.f, glm::vec2(-7, -8));
	Ghost->AddComponent(BoxColliderComponentGhost);
	auto RenderGhost = std::make_shared<dae::RenderComponent>(Ghost, "ghost.png");
	Ghost->AddComponent(RenderGhost);
	auto fovComponent = std::make_shared<dae::FOVComponent>(Ghost, 50.0f, 100.f);
	Ghost->AddComponent(fovComponent);

	auto patrolState = new FSMStates::Patrol(Ghost);
	auto chaseState = new FSMStates::ChasePlayer(Ghost);
	auto StateMachine = std::make_shared<dae::FiniteStateMachine>(Ghost, patrolState);

	auto playerInSightCondition = new FSMConditions::PlayerInSight(Ghost);
	auto playerNotInSightCondition = new FSMConditions::PlayerNotInSight(Ghost);
	StateMachine->AddTransition(patrolState, chaseState, playerInSightCondition);
	StateMachine->AddTransition(chaseState, patrolState, playerNotInSightCondition);

	Ghost->AddComponent(StateMachine);

	

	PointComponentGhost->GetPointSubject()->AddObserver(ScoreObserverGhost);
	HealthComponentGhost->GetHealthSubject()->AddObserver(HealthObserverGhost);

	transform->SetLocalPosition(120, 200, 0);

	scene.Add(Ghost);



	//const float player1Speed{ 50.f };
	auto& input = dae::InputManager::GetInstance();

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(PacMan, glm::f32vec2{ 0.f, 1.f }, 90.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(PacMan, glm::f32vec2{ 1.f, 0.f }, 0.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(PacMan, glm::f32vec2{ 0.f, -1.f }, 270.f), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(PacMan, glm::f32vec2{ -1.f, 0.f }, 180.f), dae::KeyState::keyDown);

	input.SetKeyboardCommand(SDL_SCANCODE_Z, new dae::AddPointsCommand(PacMan, 10), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_X, new dae::AddPointsCommand(PacMan, 100), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_C, new dae::RemoveHealthCommand(PacMan), dae::KeyState::keyDown);
	//input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 0.f, 1.f }), dae::KeyState::keyPressed);
	//input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 1.f, 0.f }), dae::KeyState::keyPressed);
	//input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 0.f, -1.f }), dae::KeyState::keyPressed);
	//input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ -1.f, 0.f }), dae::KeyState::keyPressed);

	/*input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonA, new dae::AddPointsCommand(PacMan, 10), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonB, new dae::AddPointsCommand(PacMan, 100), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonX, new dae::RemoveHealthCommand(PacMan), dae::KeyState::keyDown);*/

	//input.SetGamePadStickCommand(dae::GamePad::ControllerStick::LeftStick, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ -1.f, 0.f }, true));

	//input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(Ghost, glm::f32vec2{ 0.f, 1.f }), dae::KeyState::keyDown);
	//input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(Ghost, glm::f32vec2{ 1.f, 0.f }), dae::KeyState::keyDown);
	//input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(Ghost, glm::f32vec2{ 0.f, -1.f }), dae::KeyState::keyDown);
	//input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(Ghost, glm::f32vec2{ -1.f, 0.f }), dae::KeyState::keyDown);

	std::vector<glm::vec3> coinPositions = {
	{0, 300, 0},
	{50, 350, 0},
	{100, 300, 0},
	{150, 350, 0},
	{200, 300, 0},
	{250, 350, 0},
	{300, 300, 0},
	{350, 350, 0},
	{400, 300, 0},
	{450, 350, 0},
	{500, 300, 0},
	{550, 350, 0},
	{600, 300, 0},
	};


	for (size_t i = 0; i < coinPositions.size(); ++i)
	{
		auto coin = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(coin);
		coin->AddComponent(transform);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(coin, 10.f, 10.f, glm::vec2(-8, -7));
		coin->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, 100);
		coin->AddComponent(collectableComponent);

		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coin.png");
		coin->AddComponent(renderComponent);

		transform->SetLocalPosition(coinPositions[i]);

		scene.Add(coin);
	}
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
