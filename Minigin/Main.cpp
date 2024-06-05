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
	auto RotatePacMan = std::make_shared<dae::RotationComponent>(PacMan, 200.f);
	auto RenderPacMan = std::make_shared<dae::RenderComponent>(PacMan, "pacman.png");

	PacMan->AddComponent(transform);
	PacMan->AddComponent(RotatePacMan);
	PacMan->AddComponent(RenderPacMan);

	PacMan->SetParent(center, false);
	scene.Add(PacMan);



	auto Ghost = std::make_shared<dae::GameObject>();

	transform = std::make_shared<dae::TransformComponent>(Ghost);
	auto RotateGhost = std::make_shared<dae::RotationComponent>(Ghost, 150.f, false);
	auto RenderGhost = std::make_shared<dae::RenderComponent>(Ghost, "ghost.png");

	Ghost->AddComponent(transform);
	Ghost->AddComponent(RotateGhost);
	Ghost->AddComponent(RenderGhost);

	Ghost->SetParent(PacMan, false);
	scene.Add(Ghost);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}