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
	FPSCounter->AddComponent(transform);
	auto RenderTextFps = std::make_shared<dae::RenderComponent>(FPSCounter);
	FPSCounter->AddComponent(RenderTextFps);
	auto TextFps = std::make_shared<dae::TextComponent>(FPSCounter, "00 FPS", font, SDL_Color{ 255, 255, 255, 255 });
	FPSCounter->AddComponent(TextFps);
	auto FPS = std::make_shared<dae::FPSComponent>(FPSCounter);
	FPSCounter->AddComponent(FPS);

	scene.Add(FPSCounter);



	auto TextTitelObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	transform = std::make_shared<dae::TransformComponent>(TextTitelObject);
	TextTitelObject->AddComponent(transform);
	auto RenderTextTitel = std::make_shared<dae::RenderComponent>(TextTitelObject);
	TextTitelObject->AddComponent(RenderTextTitel);
	std::shared_ptr<dae::TextComponent> textTitel = std::make_shared<dae::TextComponent>(TextTitelObject, "Programming 4 Assignment", font, SDL_Color{ 255, 255, 255, 255 });
	TextTitelObject->AddComponent(textTitel);
	transform->SetLocalPosition(80, 30, 0);

	scene.Add(TextTitelObject);


	auto TextPlayer1 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer1);
	TextPlayer1->AddComponent(transform);
	auto RenderTextTitelPlayer1 = std::make_shared<dae::RenderComponent>(TextPlayer1);
	TextPlayer1->AddComponent(RenderTextTitelPlayer1);
	std::shared_ptr<dae::TextComponent> textTitelPlayer1 = std::make_shared<dae::TextComponent>(TextPlayer1, "Use the D-Pad to move Pacman, X to inflict damage, A and B to pick up pellets", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer1->AddComponent(textTitelPlayer1);
	transform->SetLocalPosition(0, 100, 0);
	scene.Add(TextPlayer1);

	auto TextPlayer2 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer2);
	TextPlayer2->AddComponent(transform);
	auto RenderTextPlayer2 = std::make_shared<dae::RenderComponent>(TextPlayer2);
	TextPlayer2->AddComponent(RenderTextPlayer2);
	std::shared_ptr<dae::TextComponent> textTitelPlayer2 = std::make_shared<dae::TextComponent>(TextPlayer2, "Use WASD to move the Ghost, C to inflict damage, Z and X to pick up pellets", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer2->AddComponent(textTitelPlayer2);
	transform->SetLocalPosition(0, 120, 0);
	scene.Add(TextPlayer2);




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
	auto PointComponentPacMan = std::make_shared<dae::PointsComponent>(PacMan);
	PacMan->AddComponent(PointComponentPacMan);
	auto HealthComponentPacMan = std::make_shared<dae::HealthComponent>(PacMan);
	PacMan->AddComponent(HealthComponentPacMan);
	auto BoxColliderComponentPacMan = std::make_shared<dae::BoxColliderComponent>(PacMan, 20.f, 20.f, glm::vec2(-7, -8));
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
	Ghost->AddComponent(transform);
	auto PointComponentGhost = std::make_shared<dae::PointsComponent>(Ghost);
	Ghost->AddComponent(PointComponentGhost);
	auto HealthComponentGhost = std::make_shared<dae::HealthComponent>(Ghost);
	Ghost->AddComponent(HealthComponentGhost);
	auto BoxColliderComponentGhost = std::make_shared<dae::BoxColliderComponent>(Ghost, 20.f, 20.f, glm::vec2(-7, -8));
	Ghost->AddComponent(BoxColliderComponentGhost);
	auto RenderGhost = std::make_shared<dae::RenderComponent>(Ghost, "ghost.png");
	Ghost->AddComponent(RenderGhost);

	PointComponentGhost->GetPointSubject()->AddObserver(ScoreObserverGhost);
	HealthComponentGhost->GetHealthSubject()->AddObserver(HealthObserverGhost);

	transform->SetLocalPosition(120, 200, 0);

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

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, dae::CollectableInfo(dae::Object::COIN, 100));
		coin->AddComponent(collectableComponent);

		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coin.png");
		coin->AddComponent(renderComponent);

		transform->SetLocalPosition(coinPositions[i]);

		scene.Add(coin);
	}
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}