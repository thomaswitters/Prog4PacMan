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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	/*auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);*/


	auto FPSCounter = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);
	std::shared_ptr<dae::FPSComponent> FPS = std::make_shared<dae::FPSComponent>(FPSCounter, font);

	FPSCounter->AddComponent(FPS);
	FPSCounter->SetPosition(250, 100);

	scene.Add(FPSCounter);
	

	auto TextObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	std::shared_ptr<dae::TextComponent> text = std::make_shared<dae::TextComponent>(TextObject, "Programming 4 Assignment", font, SDL_Color{ 255, 255, 255, 255 });

	TextObject->AddComponent(text);
	TextObject->SetPosition(80, 20);

	scene.Add(TextObject);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}