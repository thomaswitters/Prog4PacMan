#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "ScoreBoardComponent.h"
#include "PointsComponent.h"
#include "HealthBoardComponent.h"
#include "HealthComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");



	auto go = std::make_shared<dae::GameObject>();

	auto transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderTexture = std::make_shared<dae::RenderComponent>(go, "background.tga");

	go->AddComponent(transform);
	go->AddComponent(RenderTexture);

	scene.Add(go);



	go = std::make_shared<dae::GameObject>();

	transform = std::make_shared<dae::TransformComponent>(go);
	auto RenderLogo = std::make_shared<dae::RenderComponent>(go, "logo.tga");

	go->AddComponent(transform);
	go->AddComponent(RenderLogo);
	transform->SetLocalPosition(216, 180, 0);
	scene.Add(go);



	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	transform = std::make_shared<dae::TransformComponent>(FPSCounter);
	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);

	FPSCounter->AddComponent(transform);
	FPSCounter->AddComponent(RenderTextFps);
	FPSCounter->AddComponent(TextFps);
	FPSCounter->AddComponent(FPS);
	scene.Add(FPSCounter);



	auto TextTitelObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	transform = std::make_shared<dae::TransformComponent>(TextTitelObject);
	auto RenderTextTitel = std::make_shared<dae::RenderComponent>(TextTitelObject);
	std::shared_ptr<dae::TextComponent> textTitel = std::make_shared<dae::TextComponent>(TextTitelObject, "Programming 4 Assignment", font, SDL_Color{ 255, 255, 255, 255 });

	TextTitelObject->AddComponent(transform);
	TextTitelObject->AddComponent(RenderTextTitel);
	TextTitelObject->AddComponent(textTitel);

	transform->SetLocalPosition(80, 30, 0);
	scene.Add(TextTitelObject);



	auto center = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::TransformComponent>(center);
	center->AddComponent(transform);

	transform->SetLocalPosition(300, 350, 0.f);
	scene.Add(center);


	auto TextPlayer1 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer1);
	auto RenderTextTitelPlayer1 = std::make_shared<dae::RenderComponent>(TextPlayer1);
	std::shared_ptr<dae::TextComponent> textTitelPlayer1 = std::make_shared<dae::TextComponent>(TextPlayer1, "Use the D-Pad to move Pacman, X to inflict damage, A and B to pick up pellets", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer1->AddComponent(transform);
	TextPlayer1->AddComponent(RenderTextTitelPlayer1);
	TextPlayer1->AddComponent(textTitelPlayer1);
	transform->SetLocalPosition(0, 100, 0);
	scene.Add(TextPlayer1);

	auto TextPlayer2 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer2);
	auto RenderTextPlayer2 = std::make_shared<dae::RenderComponent>(TextPlayer2);
	std::shared_ptr<dae::TextComponent> textTitelPlayer2 = std::make_shared<dae::TextComponent>(TextPlayer2, "Use WASD to move the Ghost, C to inflict damage, Z and X to pick up pellets", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer2->AddComponent(transform);
	TextPlayer2->AddComponent(RenderTextPlayer2);
	TextPlayer2->AddComponent(textTitelPlayer2);
	transform->SetLocalPosition(0, 120, 0);
	scene.Add(TextPlayer2);




	auto PlayerPacManScore = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerPacManScore);
	auto RenderTextPacManScore = std::make_shared<dae::RenderComponent>(PlayerPacManScore);
	std::shared_ptr<dae::TextComponent> textPacManScore = std::make_shared<dae::TextComponent>(PlayerPacManScore, "Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerPacManScore->AddComponent(transform);
	PlayerPacManScore->AddComponent(RenderTextPacManScore);
	PlayerPacManScore->AddComponent(textPacManScore);
	transform->SetLocalPosition(0, 165, 0);
	scene.Add(PlayerPacManScore);

	auto PlayerPacManHealth = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerPacManHealth);
	auto RenderTextPlayerHealth = std::make_shared<dae::RenderComponent>(PlayerPacManHealth);
	std::shared_ptr<dae::TextComponent> textPlayerHealth = std::make_shared<dae::TextComponent>(PlayerPacManHealth, "# lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerPacManHealth->AddComponent(transform);
	PlayerPacManHealth->AddComponent(RenderTextPlayerHealth);
	PlayerPacManHealth->AddComponent(textPlayerHealth);
	transform->SetLocalPosition(0, 150, 0);
	scene.Add(PlayerPacManHealth);

	auto scoreBoard = std::make_shared<dae::ScoreBoardComponent>(PlayerPacManScore);
	auto HealthBoard = std::make_shared<dae::HealthBoardComponent>(PlayerPacManHealth);

	auto PacMan = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::TransformComponent>(PacMan);
	auto RenderPacMan = std::make_shared<dae::RenderComponent>(PacMan, "pacman.png");

	auto PointComponentPacMan = std::make_shared<dae::PointsComponent>(PacMan);
	auto HealthComponentPacMan = std::make_shared<dae::HealthComponent>(PacMan);
	PointComponentPacMan->GetPointSubject()->AddObserver(scoreBoard);
	HealthComponentPacMan->GetHealthSubject()->AddObserver(HealthBoard);

	PacMan->AddComponent(PointComponentPacMan);
	PacMan->AddComponent(HealthComponentPacMan);

	PacMan->AddComponent(transform);
	PacMan->AddComponent(RenderPacMan);
	transform->SetLocalPosition(100, 200, 0);
	scene.Add(PacMan);




	auto PlayerGhostScore = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerGhostScore);
	auto RenderTextGhostScore = std::make_shared<dae::RenderComponent>(PlayerGhostScore);
	std::shared_ptr<dae::TextComponent> textGhostScore = std::make_shared<dae::TextComponent>(PlayerGhostScore, "Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerGhostScore->AddComponent(transform);
	PlayerGhostScore->AddComponent(RenderTextGhostScore);
	PlayerGhostScore->AddComponent(textGhostScore);
	transform->SetLocalPosition(0, 215, 0);
	scene.Add(PlayerGhostScore);

	auto PlayerGhostHealth = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(PlayerGhostHealth);
	auto RenderTextGhostHealth = std::make_shared<dae::RenderComponent>(PlayerGhostHealth);
	std::shared_ptr<dae::TextComponent> textGhostHealth = std::make_shared<dae::TextComponent>(PlayerGhostHealth, "# lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	PlayerGhostHealth->AddComponent(transform);
	PlayerGhostHealth->AddComponent(RenderTextGhostHealth);
	PlayerGhostHealth->AddComponent(textGhostHealth);
	transform->SetLocalPosition(0, 200, 0);
	scene.Add(PlayerGhostHealth);

	auto scoreBoardGhost = std::make_shared<dae::ScoreBoardComponent>(PlayerGhostScore);
	auto HealthBoardGhost = std::make_shared<dae::HealthBoardComponent>(PlayerGhostHealth);

	auto Ghost = std::make_shared<dae::GameObject>();
	transform = std::make_shared<dae::TransformComponent>(Ghost);
	auto RenderGhost = std::make_shared<dae::RenderComponent>(Ghost, "ghost.png");

	auto PointComponentGhost = std::make_shared<dae::PointsComponent>(Ghost);
	auto HealthComponentGhost = std::make_shared<dae::HealthComponent>(Ghost);
	PointComponentGhost->GetPointSubject()->AddObserver(scoreBoardGhost);
	HealthComponentGhost->GetHealthSubject()->AddObserver(HealthBoardGhost);

	Ghost->AddComponent(PointComponentGhost);
	Ghost->AddComponent(HealthComponentGhost);

	Ghost->AddComponent(transform);
	Ghost->AddComponent(RenderGhost);
	transform->SetLocalPosition(120, 200, 0);
	//Ghost->SetParent(PacMan, false);
	scene.Add(Ghost);



	const float player1Speed{ 50.f };
	auto& input = dae::InputManager::GetInstance();

	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Down, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 0.f, 1.f }), dae::KeyState::keyPressed);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Right, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 1.f, 0.f }), dae::KeyState::keyPressed);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Up, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ 0.f, -1.f }), dae::KeyState::keyPressed);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::Dpad_Left, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ -1.f, 0.f }), dae::KeyState::keyPressed);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonA, new dae::AddPointsCommand(PacMan, 10), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonB, new dae::AddPointsCommand(PacMan, 100), dae::KeyState::keyDown);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonX, new dae::RemoveHealthCommand(PacMan), dae::KeyState::keyDown);

	input.SetGamePadStickCommand(dae::GamePad::ControllerStick::LeftStick, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ -1.f, 0.f }, true));


	const float player2Speed{ 100.f };

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 0.f, 1.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 1.f, 0.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 0.f, -1.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ -1.f, 0.f }), dae::KeyState::keyPressed);

	input.SetKeyboardCommand(SDL_SCANCODE_Z, new dae::AddPointsCommand(Ghost, 10), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_X, new dae::AddPointsCommand(Ghost, 100), dae::KeyState::keyDown);
	input.SetKeyboardCommand(SDL_SCANCODE_C, new dae::RemoveHealthCommand(Ghost), dae::KeyState::keyDown);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}