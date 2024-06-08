#pragma once
#include "Minigin.h"
#include "Peetje.h"
#include "GameObject.h"
#include <TextComponent.h>
#include <ScoreObserver.h>
#include <HealthObserver.h>
#include "ResourceManager.h"
#include <PacManMoveComponent.h>
#include <PointsComponent.h>
#include <HealthComponent.h>
#include <BoxColliderComponent.h>
#include <PoweredUpComponent.h>

namespace dae
{
    class Peetje
    {
    public:
        Peetje(dae::Scene& scene, std::string texturePath, int initialScore, int initialHealth, int beginCellRow, int beginCellCol)
            : m_Scene(scene), m_RenderPath(texturePath), m_InitialScore(initialScore), m_InitialHealth(initialHealth),
            m_BeginCellRow(beginCellRow), m_BeginCellCol(beginCellCol)
        {
            Initialize();
        }

        void Initialize()
        {
            m_Health = m_InitialHealth;
            m_Score = m_InitialScore;

            m_FontPath = "Lingua.otf";
            InitializeScoreAndHealth();
            SpawnPacMan();
        }


        void InitializeScoreAndHealth()
        {
            auto font = ResourceManager::GetInstance().LoadFont(m_FontPath, 18);

            // Initialize GameObject for Pac-Man's score
            m_PlayerScore = std::make_shared<GameObject>();
            auto transformComponent = std::make_shared<TransformComponent>(m_PlayerScore);
            m_PlayerScore->AddComponent(transformComponent);
            auto renderTextScore = std::make_shared<RenderComponent>(m_PlayerScore);
            m_PlayerScore->AddComponent(renderTextScore);
            m_TextScore = std::make_shared<TextComponent>(m_PlayerScore, "SCORE: " + std::to_string(m_Score), font, SDL_Color{ 255, 255, 255, 255 });
            m_PlayerScore->AddComponent(m_TextScore);
            transformComponent->SetLocalPosition(510, 60, 0);
            m_Scene.Add(m_PlayerScore);

            // Initialize GameObject for Pac-Man's health
            m_PlayerHealth = std::make_shared<GameObject>();
            transformComponent = std::make_shared<TransformComponent>(m_PlayerHealth);
            m_PlayerHealth->AddComponent(transformComponent);
            auto renderTextHealth = std::make_shared<RenderComponent>(m_PlayerHealth);
            m_PlayerHealth->AddComponent(renderTextHealth);
            m_TextHealth = std::make_shared<TextComponent>(m_PlayerHealth, "# LIVES: " + std::to_string(m_Health), font, SDL_Color{ 255, 255, 255, 255 });
            m_PlayerHealth->AddComponent(m_TextHealth);
            transformComponent->SetLocalPosition(510, 40, 0);
            m_Scene.Add(m_PlayerHealth);
        }

        void SpawnPacMan()
        {
            m_PacMan = std::make_shared<GameObject>();
            m_PacMan->SetTag("Player");

            auto transformComponent = std::make_shared<TransformComponent>(m_PacMan);
            m_PacMan->AddComponent(transformComponent);
            auto renderComponent = std::make_shared<RenderComponent>(m_PacMan, m_RenderPath);
            m_PacMan->AddComponent(renderComponent);
            auto colliderComponent = std::make_shared<BoxColliderComponent>(m_PacMan, 13.f, 13.f, glm::vec2(-renderComponent->GetTexture()->GetSize().x / 2, -renderComponent->GetTexture()->GetSize().y / 2));
            m_PacMan->AddComponent(colliderComponent);
            m_MoveComponent = std::make_shared<PacManMoveComponent>(m_PacMan, 120.f, m_BeginCellRow, m_BeginCellCol);
            m_PacMan->AddComponent(m_MoveComponent);
            auto poweredUpComponent = std::make_shared<PoweredUpComponent>(m_PacMan, 5.f);
            m_PacMan->AddComponent(poweredUpComponent);

            m_PointComponent = std::make_shared<PointsComponent>(m_PacMan, m_Score);
            m_PacMan->AddComponent(m_PointComponent);
            m_HealthComponent = std::make_shared<HealthComponent>(m_PacMan, m_Health);
            m_PacMan->AddComponent(m_HealthComponent);

            m_ScoreObserver = std::make_shared<ScoreObserver>(m_PlayerScore);
            m_HealthObserver = std::make_shared<HealthObserver>(m_PlayerHealth);
            m_PointComponent->GetPointSubject()->AddObserver(m_ScoreObserver);
            m_HealthComponent->GetHealthSubject()->AddObserver(m_HealthObserver);

            m_Scene.Add(m_PacMan);
        }

        void RespawnPacMan()
        {
            m_Scene.Remove(m_PacMan);
            SpawnPacMan();
        }

        void RespawnScoreAndHealth()
        {
            m_Health = m_HealthComponent->GetHealth();
            m_Score = m_PointComponent->GetPoints();

            m_Scene.Remove(m_PlayerScore);
            m_Scene.Remove(m_PlayerHealth);
            InitializeScoreAndHealth();
        }

        std::shared_ptr<GameObject> GetPacMan()
        {
            return m_PacMan;
        }

    private:
        Scene& m_Scene;
        std::string m_RenderPath;
        int m_InitialScore;
        int m_InitialHealth;
        int m_Score;
        int m_Health;

        std::string m_FontPath;
        std::shared_ptr<GameObject> m_PlayerScore;
        std::shared_ptr<TextComponent> m_TextScore;
        std::shared_ptr<GameObject> m_PlayerHealth;
        std::shared_ptr<TextComponent> m_TextHealth;
        std::shared_ptr<GameObject> m_PacMan;
        std::shared_ptr<ScoreObserver> m_ScoreObserver;
        std::shared_ptr<HealthObserver> m_HealthObserver;
        std::shared_ptr<TransformComponent> m_TransFormComponent;
        std::shared_ptr<PacManMoveComponent> m_MoveComponent;
        std::shared_ptr<PointsComponent> m_PointComponent;
        std::shared_ptr<HealthComponent> m_HealthComponent;

        int m_BeginCellRow;
        int m_BeginCellCol;
    };
}
