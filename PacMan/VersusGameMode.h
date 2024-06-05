#pragma once
#include <Scene.h>
#include <RenderComponent.h>
#include <TextComponent.h>
#include <FPSComponent.h>
#include "Peetje.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include <CollectableComponent.h>
#include <GhosStatesAndTransitions.h>

using json = nlohmann::json;

namespace dae
{
	class VersusGameMode : public GameMode
	{
	public:
		VersusGameMode(Scene& scene) : GameMode(scene) {}

		void SetupGameMode() override;
		void InitializeGhost(std::shared_ptr<dae::GameObject> ghost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, FSMStates::ChasePlayer::FindPathType pathType) override;
		std::vector<glm::vec3> LoadPositionsFromJSON(const std::string& filePath, const std::string& type) override;
	};
}

