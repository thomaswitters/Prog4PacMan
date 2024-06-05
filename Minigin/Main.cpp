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

	auto PacMan = std::make_shared<dae::GameObject>();

	transform = std::make_shared<dae::TransformComponent>(PacMan);
	//auto RotatePacMan = std::make_shared<dae::RotationComponent>(PacMan, 250.f);
	auto RenderPacMan = std::make_shared<dae::RenderComponent>(PacMan, "pacman.png");

	PacMan->AddComponent(transform);
	//PacMan->AddComponent(RotatePacMan);
	PacMan->AddComponent(RenderPacMan);

	transform->SetLocalPosition(100, 200, 0);
	//PacMan->SetParent(center, false);
	scene.Add(PacMan);



	auto Ghost = std::make_shared<dae::GameObject>();

	transform = std::make_shared<dae::TransformComponent>(Ghost);
	auto RotateGhost = std::make_shared<dae::RotationComponent>(Ghost, 150.f, false);
	auto RenderGhost = std::make_shared<dae::RenderComponent>(Ghost, "ghost.png");

	Ghost->AddComponent(transform);
	//Ghost->AddComponent(RotateGhost);
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

	//input.SetGamePadStickCommand(dae::GamePad::ControllerStick::LeftStick, new dae::MoveCommand(PacMan, player1Speed, glm::f32vec2{ -1.f, 0.f }, true));


	// input
	const float player2Speed{ 100.f };

	input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 0.f, 1.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 1.f, 0.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ 0.f, -1.f }), dae::KeyState::keyPressed);
	input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::MoveCommand(Ghost, player2Speed, glm::f32vec2{ -1.f, 0.f }), dae::KeyState::keyPressed);


	auto TextPlayer1 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer1);
	auto RenderTextTitelPlayer1 = std::make_shared<dae::RenderComponent>(TextPlayer1);
	std::shared_ptr<dae::TextComponent> textTitelPlayer1 = std::make_shared<dae::TextComponent>(TextPlayer1, "Use the D-Pad to move Pacman", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer1->AddComponent(transform);
	TextPlayer1->AddComponent(RenderTextTitelPlayer1);
	TextPlayer1->AddComponent(textTitelPlayer1);
	transform->SetLocalPosition(0, 100, 0);
	scene.Add(TextPlayer1);

	auto TextPlayer2 = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	transform = std::make_shared<dae::TransformComponent>(TextPlayer2);
	auto RenderTextPlayer2 = std::make_shared<dae::RenderComponent>(TextPlayer2);
	std::shared_ptr<dae::TextComponent> textPlayer2 = std::make_shared<dae::TextComponent>(TextPlayer2, "Use WASD to move the Ghost", font, SDL_Color{ 255, 255, 255, 255 });
	TextPlayer2->AddComponent(transform);
	TextPlayer2->AddComponent(RenderTextPlayer2);
	TextPlayer2->AddComponent(textPlayer2);
	transform->SetLocalPosition(0, 120, 0);
	scene.Add(TextPlayer2);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}