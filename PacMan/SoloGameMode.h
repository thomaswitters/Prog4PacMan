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
#include "Peetje.h"
#include <memory>
#include "HighscoreManager.h"

using json = nlohmann::json;

namespace dae
{
	class SoloGameMode : public GameMode
    {
    public:
        SoloGameMode(Scene& scene) : GameMode(scene) {}

        void SetupGameMode() override;
        void SetupLevel2();
        void SetupLevel3();

		void SetupHighScoreScreen();
		void SetupLosHighScoreScreen() override;

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
				SetupHighScoreScreen();
				break;
			}
        };

		int GetAmountOfLevels() const override {return m_AmountLevels;}
		int GetCurrentLevel() const override{return m_CurrentLevel;}
	private:
		void SetupLevel(const std::string& levelTexture, float ghostChaseTime);
		void SetupGhosts(float chaseTime);
		void SetupCollectables(const std::string& filePath, const std::string& key, const std::string& texture, float width, float height, dae::Object type, int value);
		void SetupFPSCounter();
		void SetupInputCommands();

		void SetupHighscores();

		void InitializeGhost(std::shared_ptr<dae::GameObject> pGhost, std::string texturePath, std::vector<int> patrolPoints, float maxTimeInBase, float maxTimeInChase, FSMStates::ChasePlayer::FindPathType pathType) override;
		std::vector<glm::vec3> LoadPositionsFromJSON(const std::string& filePath, const std::string& type) override;

		std::shared_ptr<Peetje> m_pPacMan;

		int m_AmountLevels = 3;
		int m_CurrentLevel = 0;

		int m_Score = 0;
    };
}

