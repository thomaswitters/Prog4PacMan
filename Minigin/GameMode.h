#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <string>
#include "Scene.h"
#include <json.hpp>
#include "GhosStatesAndTransitions.h"

namespace dae
{
	class GameMode
	{
	public:
		GameMode(Scene& scene) : m_Scene(scene) {}
		virtual ~GameMode() = default;

		virtual void SetupGameMode() = 0;
		virtual void InitializeGhost(std::shared_ptr<dae::GameObject> ghost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, FSMStates::ChasePlayer::FindPathType pathType) = 0;
		virtual std::vector<glm::vec3> LoadPositionsFromJSON(const std::string& filePath, const std::string& type) = 0;

	protected:
		Scene& m_Scene;
	};
}

