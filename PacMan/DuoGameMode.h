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
#include "GameMode.h"

using json = nlohmann::json;

namespace dae
{
	class DuoGameMode : public GameMode
	{
	public:
		DuoGameMode(Scene& scene) : GameMode(scene) {}

		void SetupGameMode() override;
		void SetupLevel2();
		void SetupLevel3();

		void NextLevel() override {
			m_CurrentLevel++;
			switch (m_CurrentLevel)
			{
			case 1:
			{
				SetupGameMode();
				break;
			}
			case 2:
			{
				SetupLevel2();
				break;
			}
			case 3:
			{
				SetupLevel3();
				break;
			}
			default:
				break;
			}
		};


		void InitializeGhost(std::shared_ptr<dae::GameObject> pGhost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, float maxTimeInChase, FSMStates::ChasePlayer::FindPathType pathType) override;
		std::vector<glm::vec3> LoadPositionsFromJSON(const std::string& filePath, const std::string& type) override;

		int GetAmountOfLevels() const override { return m_AmountLevels; } 
		int GetCurrentLevel() const override { return m_CurrentLevel; }
	private:
		std::shared_ptr<Peetje> m_PacMan;
		std::shared_ptr<Peetje> m_MsPacMan;

		int m_AmountLevels = 3;
		int m_CurrentLevel = 0;
	};
}
