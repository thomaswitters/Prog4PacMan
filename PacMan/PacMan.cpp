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
#include "Peetje.h"
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

std::vector<glm::vec3> LoadCoinPositionsFromJSON(const std::string& filePath) {
	std::vector<glm::vec3> positions;
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return positions;
	}

	json j;
	file >> j;

	for (const auto& item : j) {
		if (item.is_array() && item.size() == 3) {
			float x = item[0].get<float>();
			float y = item[1].get<float>();
			float z = item[2].get<float>();
			positions.emplace_back(x, y, z);
		}
	}

	return positions;
}

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


	Peetje pacMan(scene, 0, 3);

	

	std::string filePath = "vertices.json";
	std::vector<glm::vec3> coinPositions = LoadCoinPositionsFromJSON(filePath);


	for (size_t i = 0; i < coinPositions.size(); ++i)
	{
		auto coin = std::make_shared<dae::GameObject>();

		transform = std::make_shared<dae::TransformComponent>(coin);
		coin->AddComponent(transform);

		auto boxCollider = std::make_shared<dae::BoxColliderComponent>(coin, 5.f, 5.f, glm::vec2(-8, -7));
		coin->AddComponent(boxCollider);

		auto collectableComponent = std::make_shared<dae::CollectableComponent>(coin, dae::CollectableInfo(dae::Object::Coin, 100));
		coin->AddComponent(collectableComponent);

		auto renderComponent = std::make_shared<dae::RenderComponent>(coin, "coinSmal.png");
		coin->AddComponent(renderComponent);

		transform->SetLocalPosition(coinPositions[i]);

		scene.Add(coin);
	}

	{
		auto Ghost1 = std::make_shared<dae::GameObject>();
		Ghost1->SetTag("Ghost");

		auto transform1 = std::make_shared<dae::TransformComponent>(Ghost1);
		Ghost1->AddComponent(transform1);
		auto PointComponentGhost1 = std::make_shared<dae::PointsComponent>(Ghost1);
		Ghost1->AddComponent(PointComponentGhost1);
		auto HealthComponentGhost1 = std::make_shared<dae::HealthComponent>(Ghost1);
		Ghost1->AddComponent(HealthComponentGhost1);
		auto BoxColliderComponentGhost1 = std::make_shared<dae::BoxColliderComponent>(Ghost1, 20.f, 20.f, glm::vec2(-7, -8));
		Ghost1->AddComponent(BoxColliderComponentGhost1);
		auto GhostcollectableComponent1 = std::make_shared<dae::CollectableComponent>(Ghost1, dae::CollectableInfo(dae::Object::Ghost, 1));
		Ghost1->AddComponent(GhostcollectableComponent1);
		auto fovComponent1 = std::make_shared<dae::FOVComponent>(Ghost1, 360.0f, 100.f);
		Ghost1->AddComponent(fovComponent1);
		auto m_MoveComponent1 = std::make_shared<PacManMoveComponent>(Ghost1, 60.f, 4, 2);
		Ghost1->AddComponent(m_MoveComponent1);
		auto RenderGhost1 = std::make_shared<dae::RenderComponent>(Ghost1, "ghost.png");
		Ghost1->AddComponent(RenderGhost1);

		auto patrolState1 = new FSMStates::Patrol(Ghost1, std::vector<int>{0, 7, 41, 34});
		auto chaseState1 = new FSMStates::ChasePlayer(Ghost1, FSMStates::ChasePlayer::FindPathType::BESTPATH);
		auto StateMachine1 = std::make_shared<dae::FiniteStateMachine>(Ghost1, patrolState1);

		auto playerInSightCondition1 = new FSMConditions::PlayerInSight(Ghost1);
		auto playerNotInSightCondition1 = new FSMConditions::PlayerNotInSight(Ghost1);
		StateMachine1->AddTransition(patrolState1, chaseState1, playerInSightCondition1);
		StateMachine1->AddTransition(chaseState1, patrolState1, playerNotInSightCondition1);

		Ghost1->AddComponent(StateMachine1);

		scene.Add(Ghost1);
	}

	// For Ghost 2
	{
		auto Ghost2 = std::make_shared<dae::GameObject>();
		Ghost2->SetTag("Ghost");

		auto transform2 = std::make_shared<dae::TransformComponent>(Ghost2);
		Ghost2->AddComponent(transform2);
		auto PointComponentGhost2 = std::make_shared<dae::PointsComponent>(Ghost2);
		Ghost2->AddComponent(PointComponentGhost2);
		auto HealthComponentGhost2 = std::make_shared<dae::HealthComponent>(Ghost2);
		Ghost2->AddComponent(HealthComponentGhost2);
		auto BoxColliderComponentGhost2 = std::make_shared<dae::BoxColliderComponent>(Ghost2, 20.f, 20.f, glm::vec2(-7, -8));
		Ghost2->AddComponent(BoxColliderComponentGhost2);
		auto GhostcollectableComponent2 = std::make_shared<dae::CollectableComponent>(Ghost2, dae::CollectableInfo(dae::Object::Ghost, 1));
		Ghost2->AddComponent(GhostcollectableComponent2);
		auto fovComponent2 = std::make_shared<dae::FOVComponent>(Ghost2, 360.0f, 100.f);
		Ghost2->AddComponent(fovComponent2);
		auto m_MoveComponent2 = std::make_shared<PacManMoveComponent>(Ghost2, 60.f, 4, 16);
		Ghost2->AddComponent(m_MoveComponent2);
		auto RenderGhost2 = std::make_shared<dae::RenderComponent>(Ghost2, "ghost2.png");
		Ghost2->AddComponent(RenderGhost2);

		auto patrolState2 = new FSMStates::Patrol(Ghost2, std::vector<int>{16, 9, 43, 49});
		auto chaseState2 = new FSMStates::ChasePlayer(Ghost2, FSMStates::ChasePlayer::FindPathType::SECONDBESTPATH);
		auto StateMachine2 = std::make_shared<dae::FiniteStateMachine>(Ghost2, patrolState2);

		auto playerInSightCondition2 = new FSMConditions::PlayerInSight(Ghost2);
		auto playerNotInSightCondition2 = new FSMConditions::PlayerNotInSight(Ghost2);
		StateMachine2->AddTransition(patrolState2, chaseState2, playerInSightCondition2);
		StateMachine2->AddTransition(chaseState2, patrolState2, playerNotInSightCondition2);

		Ghost2->AddComponent(StateMachine2);

		scene.Add(Ghost2);
	}

	{
		auto Ghost3 = std::make_shared<dae::GameObject>();
		Ghost3->SetTag("Ghost");

		auto transform3 = std::make_shared<dae::TransformComponent>(Ghost3);
		Ghost3->AddComponent(transform3);
		auto PointComponentGhost3 = std::make_shared<dae::PointsComponent>(Ghost3);
		Ghost3->AddComponent(PointComponentGhost3);
		auto HealthComponentGhost3 = std::make_shared<dae::HealthComponent>(Ghost3);
		Ghost3->AddComponent(HealthComponentGhost3);
		auto BoxColliderComponentGhost3 = std::make_shared<dae::BoxColliderComponent>(Ghost3, 20.f, 20.f, glm::vec2(-7, -8));
		Ghost3->AddComponent(BoxColliderComponentGhost3);
		auto GhostcollectableComponent3 = std::make_shared<dae::CollectableComponent>(Ghost3, dae::CollectableInfo(dae::Object::Ghost, 1));
		Ghost3->AddComponent(GhostcollectableComponent3);
		auto fovComponent3 = std::make_shared<dae::FOVComponent>(Ghost3, 360.0f, 100.f);
		Ghost3->AddComponent(fovComponent3);
		auto m_MoveComponent3 = std::make_shared<PacManMoveComponent>(Ghost3, 60.f, 18, 16);
		Ghost3->AddComponent(m_MoveComponent3);
		auto RenderGhost3 = std::make_shared<dae::RenderComponent>(Ghost3, "ghost3.png");
		Ghost3->AddComponent(RenderGhost3);

		auto patrolState3 = new FSMStates::Patrol(Ghost3, std::vector<int>{322, 315, 250, 305});
		auto chaseState3 = new FSMStates::ChasePlayer(Ghost3, FSMStates::ChasePlayer::FindPathType::SECONDBESTPATH);
		auto StateMachine3 = std::make_shared<dae::FiniteStateMachine>(Ghost3, patrolState3);

		auto playerInSightCondition3 = new FSMConditions::PlayerInSight(Ghost3);
		auto playerNotInSightCondition3 = new FSMConditions::PlayerNotInSight(Ghost3);
		StateMachine3->AddTransition(patrolState3, chaseState3, playerInSightCondition3);
		StateMachine3->AddTransition(chaseState3, patrolState3, playerNotInSightCondition3);

		Ghost3->AddComponent(StateMachine3);

		scene.Add(Ghost3);
	}

	{
		auto Ghost4 = std::make_shared<dae::GameObject>();
		Ghost4->SetTag("Ghost");

		auto transform4 = std::make_shared<dae::TransformComponent>(Ghost4);
		Ghost4->AddComponent(transform4);
		auto PointComponentGhost4 = std::make_shared<dae::PointsComponent>(Ghost4);
		Ghost4->AddComponent(PointComponentGhost4);
		auto HealthComponentGhost4 = std::make_shared<dae::HealthComponent>(Ghost4);
		Ghost4->AddComponent(HealthComponentGhost4);
		auto BoxColliderComponentGhost4 = std::make_shared<dae::BoxColliderComponent>(Ghost4, 20.f, 20.f, glm::vec2(-7, -8));
		Ghost4->AddComponent(BoxColliderComponentGhost4);
		auto GhostcollectableComponent4 = std::make_shared<dae::CollectableComponent>(Ghost4, dae::CollectableInfo(dae::Object::Ghost, 1));
		Ghost4->AddComponent(GhostcollectableComponent4);
		auto fovComponent4 = std::make_shared<dae::FOVComponent>(Ghost4, 360.0f, 100.f);
		Ghost4->AddComponent(fovComponent4);
		auto m_MoveComponent4 = std::make_shared<PacManMoveComponent>(Ghost4, 60.f, 18, 5);
		Ghost4->AddComponent(m_MoveComponent4);
		auto RenderGhost4 = std::make_shared<dae::RenderComponent>(Ghost4, "ghost4.png");
		Ghost4->AddComponent(RenderGhost4);

		auto patrolState4 = new FSMStates::Patrol(Ghost4, std::vector<int>{306, 313, 242, 289});
		auto chaseState4 = new FSMStates::ChasePlayer(Ghost4, FSMStates::ChasePlayer::FindPathType::BESTPATH);
		auto StateMachine4 = std::make_shared<dae::FiniteStateMachine>(Ghost4, patrolState4);

		auto playerInSightCondition4 = new FSMConditions::PlayerInSight(Ghost4);
		auto playerNotInSightCondition4 = new FSMConditions::PlayerNotInSight(Ghost4);
		StateMachine4->AddTransition(patrolState4, chaseState4, playerInSightCondition4);
		StateMachine4->AddTransition(chaseState4, patrolState4, playerNotInSightCondition4);

		Ghost4->AddComponent(StateMachine4);

		scene.Add(Ghost4);
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
