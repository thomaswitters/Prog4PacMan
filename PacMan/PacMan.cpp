// PacMan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
#include <FiniteStateMachineComponent.h>
#include <GhosStatesAndTransitions.h>
#include <FOVComponent.h>
#include "Peetje.h"
#include <json.hpp>
#include <fstream>
#include "SoloGameMode.h"
#include "DuoGameMode.h" 
#include "VersusGameMode.h"
#include "ServiceLocator.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level1");
	auto& scene1 = dae::SceneManager::GetInstance().CreateScene("Level2");
	auto& scene2 = dae::SceneManager::GetInstance().CreateScene("Level3");
	auto& beginScene = dae::SceneManager::GetInstance().CreateScene("StartScene");
	auto& endSceneWon = dae::SceneManager::GetInstance().CreateScene("EndSceneWon");
	auto& endSceneLost = dae::SceneManager::GetInstance().CreateScene("EndSceneLost");

	dae::SceneManager::GetInstance().SetActiveScene("StartScene");

	auto BeginScene = std::make_shared<dae::GameObject>();

	auto transformBeginScene = std::make_shared<dae::TransformComponent>(BeginScene);
	auto RenderTextureBeginScene = std::make_shared<dae::RenderComponent>(BeginScene, "background.tga");
	BeginScene->AddComponent(transformBeginScene);
	BeginScene->AddComponent(RenderTextureBeginScene);
	beginScene.Add(BeginScene);

	auto PacmanLogo = std::make_shared<dae::GameObject>();

	auto transformPacmanLogo = std::make_shared<dae::TransformComponent>(PacmanLogo);
	auto RenderTexturePacmanLogo = std::make_shared<dae::RenderComponent>(PacmanLogo, "PacManLogo.png");
	PacmanLogo->AddComponent(transformPacmanLogo);
	transformPacmanLogo->SetLocalPosition(120, 30, 0);
	PacmanLogo->AddComponent(RenderTexturePacmanLogo);
	beginScene.Add(PacmanLogo);

	auto PacmanSoloLoby = std::make_shared<dae::GameObject>();

	auto transformPacmanSoloLoby = std::make_shared<dae::TransformComponent>(PacmanSoloLoby);
	auto RenderTexturePacmanSoloLoby = std::make_shared<dae::RenderComponent>(PacmanSoloLoby, "pacman.png");
	PacmanSoloLoby->AddComponent(transformPacmanSoloLoby);
	transformPacmanSoloLoby->SetLocalPosition(290, 300, 0);
	PacmanSoloLoby->AddComponent(RenderTexturePacmanSoloLoby);
	beginScene.Add(PacmanSoloLoby);

	auto StartSoloGame = std::make_shared<GameObject>();
	auto fontSoloGame = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto transFormStartSoloGame = std::make_shared<TransformComponent>(StartSoloGame);
	StartSoloGame->AddComponent(transFormStartSoloGame);
	auto renderTextStartSoloGame = std::make_shared<RenderComponent>(StartSoloGame);
	StartSoloGame->AddComponent(renderTextStartSoloGame);
	auto textStartSoloGame = std::make_shared<TextComponent>(StartSoloGame, "A/ENTER to start", fontSoloGame, SDL_Color{ 255, 255, 255, 255 });
	StartSoloGame->AddComponent(textStartSoloGame);
	transFormStartSoloGame->SetLocalPosition(250, 235, 0);
	beginScene.Add(StartSoloGame);

	auto SwitchGameMode = std::make_shared<GameObject>();
	auto fontSwitchGameMode = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto transFormSwitchGameMode = std::make_shared<TransformComponent>(SwitchGameMode);
	SwitchGameMode->AddComponent(transFormSwitchGameMode);
	auto renderTextSwitchGameMode = std::make_shared<RenderComponent>(SwitchGameMode);
	SwitchGameMode->AddComponent(renderTextSwitchGameMode);
	auto textSwitchGameMode = std::make_shared<TextComponent>(SwitchGameMode, "Y/TAb to switch gameMode", fontSoloGame, SDL_Color{ 255, 255, 255, 255 });
	StartSoloGame->AddComponent(textSwitchGameMode);
	transFormSwitchGameMode->SetLocalPosition(220, 265, 0);
	beginScene.Add(SwitchGameMode);

	//End Scene
	auto BackGround = std::make_shared<dae::GameObject>();

	auto transformBackGround = std::make_shared<dae::TransformComponent>(BackGround);
	auto RenderTextureBackGround = std::make_shared<dae::RenderComponent>(BackGround, "background.tga");
	BackGround->AddComponent(transformBackGround);
	BackGround->AddComponent(RenderTextureBackGround);
	endSceneWon.Add(BackGround);

	auto EndScreenPeetje = std::make_shared<dae::GameObject>();

	auto transformEndScreenPeetje = std::make_shared<dae::TransformComponent>(EndScreenPeetje);
	auto RenderTextureEndScreenPeetje = std::make_shared<dae::RenderComponent>(EndScreenPeetje, "EndScreenPhoto.png");
	EndScreenPeetje->AddComponent(transformEndScreenPeetje);
	transformEndScreenPeetje->SetLocalPosition(30, 80, 0);
	EndScreenPeetje->AddComponent(RenderTextureEndScreenPeetje);
	endSceneWon.Add(EndScreenPeetje);

	auto EndSceneWon = std::make_shared<GameObject>();
	auto fontEndSceneWon = ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);
	auto transFormEndSceneWon = std::make_shared<TransformComponent>(EndSceneWon);
	EndSceneWon->AddComponent(transFormEndSceneWon);
	auto renderEndSceneWon = std::make_shared<RenderComponent>(EndSceneWon);
	EndSceneWon->AddComponent(renderEndSceneWon);
	auto textEndSceneWon = std::make_shared<TextComponent>(EndSceneWon, "Congradulations You won !!!", fontEndSceneWon, SDL_Color{ 255, 255, 0, 255 });
	EndSceneWon->AddComponent(textEndSceneWon);
	transFormEndSceneWon->SetLocalPosition(340, 225, 0);
	endSceneWon.Add(EndSceneWon);


	auto BackGroundEndSceneLost = std::make_shared<dae::GameObject>();

	auto transformBackGroundEndSceneLost = std::make_shared<dae::TransformComponent>(BackGroundEndSceneLost);
	auto RenderTextureBackGroundEndSceneLost = std::make_shared<dae::RenderComponent>(BackGroundEndSceneLost, "background.tga");
	BackGroundEndSceneLost->AddComponent(transformBackGroundEndSceneLost);
	BackGroundEndSceneLost->AddComponent(RenderTextureBackGroundEndSceneLost);
	endSceneLost.Add(BackGroundEndSceneLost);

	auto EndScreenLostPeetje = std::make_shared<dae::GameObject>();

	auto transformEndScreenLostPeetje = std::make_shared<dae::TransformComponent>(EndScreenLostPeetje);
	auto RenderTextureEndScreenLostPeetje = std::make_shared<dae::RenderComponent>(EndScreenLostPeetje, "EndScreenGhost.png");
	EndScreenLostPeetje->AddComponent(transformEndScreenLostPeetje);
	transformEndScreenLostPeetje->SetLocalPosition(30, 80, 0);
	EndScreenLostPeetje->AddComponent(RenderTextureEndScreenLostPeetje);
	endSceneLost.Add(EndScreenLostPeetje);

	auto EndSceneLost = std::make_shared<GameObject>();
	auto fontEndSceneLost = ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);
	auto transFormEndSceneLost = std::make_shared<TransformComponent>(EndSceneLost);
	EndSceneLost->AddComponent(transFormEndSceneLost);
	auto renderEndSceneLost = std::make_shared<RenderComponent>(EndSceneLost);
	EndSceneLost->AddComponent(renderEndSceneLost);
	auto textEndSceneLost = std::make_shared<TextComponent>(EndSceneLost, "You Lost !!!", fontEndSceneLost, SDL_Color{ 255, 0, 0, 255 });
	EndSceneLost->AddComponent(textEndSceneLost);
	transFormEndSceneLost->SetLocalPosition(420, 225, 0);
	endSceneLost.Add(EndSceneLost); 
	
	// Game level 1

	std::shared_ptr<dae::GameMode> gameMode1 = std::make_shared<dae::SoloGameMode>(scene);
	std::shared_ptr<dae::GameMode> gameMode2 = std::make_shared<dae::DuoGameMode>(scene1);
	std::shared_ptr<dae::GameMode> gameMode3 = std::make_shared<dae::VersusGameMode>(scene2);

	std::vector<std::shared_ptr<dae::GameMode>> allGameModes;
	allGameModes.push_back(gameMode1);
	allGameModes.push_back(gameMode2);
	allGameModes.push_back(gameMode3);


	auto& switchBetweenGameModes1 = *new dae::SwitchBetweenGameModesCommand(PacmanSoloLoby);
	auto& switchBetweenGameModes2 = *new dae::SwitchBetweenGameModesCommand(PacmanSoloLoby);
	auto& input = dae::InputManager::GetInstance();

	input.SetKeyboardCommand(SDL_SCANCODE_F2, new dae::SkipLevels(), dae::KeyState::keyDown, false);
	input.SetKeyboardCommand(SDL_SCANCODE_F1, new dae::MuteAndUnMuteSounds(), dae::KeyState::keyDown, false);

	input.SetKeyboardCommand(SDL_SCANCODE_TAB, &switchBetweenGameModes1, dae::KeyState::keyDown, false);
	input.SetKeyboardCommand(SDL_SCANCODE_RETURN, new dae::StartGame(allGameModes, switchBetweenGameModes1), dae::KeyState::keyDown, false);

	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonY, &switchBetweenGameModes2, dae::KeyState::keyDown, false);
	input.SetGamePadCommand(dae::GamePad::ControllerButton::ButtonA, new dae::StartGame(allGameModes, switchBetweenGameModes2), dae::KeyState::keyDown, false);


	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	dae::ServiceLocator::GetSoundSystem().PlayMusic("../Data/Sounds/1-03. PAC-MAN NEVA PAX!!.mp3", 50, -1);
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
