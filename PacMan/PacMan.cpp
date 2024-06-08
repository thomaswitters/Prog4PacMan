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
#include <TypeNameComponent.h>
#include <TextNameObserver.h>

void load()
{
	auto& input = dae::InputManager::GetInstance();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("GameModeSolo");
	auto& scene1 = dae::SceneManager::GetInstance().CreateScene("GameModeCoop");
	auto& scene2 = dae::SceneManager::GetInstance().CreateScene("GameModeVersus");
	auto& beginScene = dae::SceneManager::GetInstance().CreateScene("StartScene");

	dae::SceneManager::GetInstance().SetActiveScene("StartScene");

	auto BackGround = std::make_shared<dae::GameObject>();

	auto transformBeginScene = std::make_shared<dae::TransformComponent>(BackGround);
	auto RenderTextureBeginScene = std::make_shared<dae::RenderComponent>(BackGround, "background.tga");
	BackGround->AddComponent(transformBeginScene);
	BackGround->AddComponent(RenderTextureBeginScene);
	beginScene.Add(BackGround);

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
	transformPacmanSoloLoby->SetLocalPosition(300, 300, 0);
	PacmanSoloLoby->AddComponent(RenderTexturePacmanSoloLoby);
	beginScene.Add(PacmanSoloLoby);

	auto StartSoloGame = std::make_shared<GameObject>();
	auto fontSoloGame = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto transFormStartSoloGame = std::make_shared<TransformComponent>(StartSoloGame);
	StartSoloGame->AddComponent(transFormStartSoloGame);
	auto renderTextStartSoloGame = std::make_shared<RenderComponent>(StartSoloGame);
	StartSoloGame->AddComponent(renderTextStartSoloGame);
	auto textStartSoloGame = std::make_shared<TextComponent>(StartSoloGame, "A/ENTER to Start", fontSoloGame, SDL_Color{ 255, 255, 255, 255 });
	StartSoloGame->AddComponent(textStartSoloGame);
	transFormStartSoloGame->SetLocalPosition(250, 235, 0);
	beginScene.Add(StartSoloGame);

	auto SwitchGameMode = std::make_shared<GameObject>();
	auto fontSwitchGameMode = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto transFormSwitchGameMode = std::make_shared<TransformComponent>(SwitchGameMode);
	SwitchGameMode->AddComponent(transFormSwitchGameMode);
	auto renderTextSwitchGameMode = std::make_shared<RenderComponent>(SwitchGameMode);
	SwitchGameMode->AddComponent(renderTextSwitchGameMode);
	auto textSwitchGameMode = std::make_shared<TextComponent>(SwitchGameMode, "Y/TAB to switch Game Mode", fontSoloGame, SDL_Color{255, 255, 255, 255});
	StartSoloGame->AddComponent(textSwitchGameMode);
	transFormSwitchGameMode->SetLocalPosition(220, 265, 0);
	beginScene.Add(SwitchGameMode);


	auto fontName = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto m_PlayerScore = std::make_shared<GameObject>();
	auto transformComponent = std::make_shared<TransformComponent>(m_PlayerScore);
	m_PlayerScore->AddComponent(transformComponent);
	auto renderTextScore = std::make_shared<RenderComponent>(m_PlayerScore);
	m_PlayerScore->AddComponent(renderTextScore);
	auto m_TextScore = std::make_shared<TextComponent>(m_PlayerScore, "NAME: Enter your name" , fontName, SDL_Color{ 255, 255, 0, 255 });
	m_PlayerScore->AddComponent(m_TextScore);
	transformComponent->SetLocalPosition(220, 190, 0);
	beginScene.Add(m_PlayerScore);

	auto m_NameComponent = std::make_shared<TypeNameComponent>(m_PlayerScore);
	m_PlayerScore->AddComponent(m_NameComponent);

	auto m_TextObserver = std::make_shared<TextNameObserver>(m_PlayerScore);
	m_NameComponent->GetNameSubject()->AddObserver(m_TextObserver);

	dae::HighscoreManager::GetInstance().SetNameObject(m_PlayerScore);
	
	std::shared_ptr<dae::GameMode> gameMode1 = std::make_shared<dae::SoloGameMode>(scene);
	std::shared_ptr<dae::GameMode> gameMode2 = std::make_shared<dae::DuoGameMode>(scene1);
	std::shared_ptr<dae::GameMode> gameMode3 = std::make_shared<dae::VersusGameMode>(scene2);

	std::vector<std::shared_ptr<dae::GameMode>> allGameModes;
	allGameModes.push_back(gameMode1);
	allGameModes.push_back(gameMode2);
	allGameModes.push_back(gameMode3);

	auto& switchBetweenGameModes1 = *new dae::SwitchBetweenGameModesCommand(PacmanSoloLoby);
	auto& switchBetweenGameModes2 = *new dae::SwitchBetweenGameModesCommand(PacmanSoloLoby);

	input.SetKeyboardCommand(SDL_SCANCODE_F1, new dae::SkipLevels(), dae::KeyState::keyDown, false);
	input.SetKeyboardCommand(SDL_SCANCODE_F2, new dae::MuteAndUnMuteSounds(), dae::KeyState::keyDown, false);

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
