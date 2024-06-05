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

namespace dae
{
	enum PlayerType {
		GhostType, PacManType
	};

    class Peetje
    {
    public:
        Peetje(dae::Scene& scene, int initialScore, int initialHealth)
            : m_Scene(scene), m_InitialScore(initialScore), m_InitialHealth(initialHealth)
        {
            Initialize();
            SetupInput();
        }

        void Initialize() {

            m_FontPath = "Lingua.otf";

            m_PlayerScore = std::make_shared<GameObject>();
            auto font = ResourceManager::GetInstance().LoadFont(m_FontPath, 14);
            m_TransFormComponent = std::make_shared<TransformComponent>(m_PlayerScore);
            m_PlayerScore->AddComponent(m_TransFormComponent);
            auto renderTextScore = std::make_shared<RenderComponent>(m_PlayerScore);
            m_PlayerScore->AddComponent(renderTextScore);
            m_TextScore = std::make_shared<TextComponent>(m_PlayerScore, "Score: " + std::to_string(m_InitialScore), font, SDL_Color{ 255, 255, 255, 255 });
            m_PlayerScore->AddComponent(m_TextScore);
            m_TransFormComponent->SetLocalPosition(0, 165, 0);
            m_Scene.Add(m_PlayerScore);

            // Create GameObject for PacMan's health
            m_PlayerHealth = std::make_shared<dae::GameObject>();
            m_TransFormComponent = std::make_shared<TransformComponent>(m_PlayerHealth);
            m_PlayerHealth->AddComponent(m_TransFormComponent);
            auto renderTextHealth = std::make_shared<RenderComponent>(m_PlayerHealth);
            m_PlayerHealth->AddComponent(renderTextHealth);
            m_TextHealth = std::make_shared<TextComponent>(m_PlayerHealth, "# lives: " + std::to_string(m_InitialHealth), font, SDL_Color{ 255, 255, 255, 255 });
            m_PlayerHealth->AddComponent(m_TextHealth);
            m_TransFormComponent->SetLocalPosition(0, 150, 0);
            m_Scene.Add(m_PlayerHealth);

            m_ScoreObserver = std::make_shared<ScoreObserver>(m_PlayerScore);
            m_HealthObserver = std::make_shared<HealthObserver>(m_PlayerHealth);
            // Create PacMan GameObject
            m_PacMan = std::make_shared<GameObject>();
            m_PacMan->SetTag("Player");

            m_TransFormComponent = std::make_shared<TransformComponent>(m_PacMan);
            m_PacMan->AddComponent(m_TransFormComponent);
            m_MoveComponent = std::make_shared<PacManMoveComponent>(m_PacMan, 60.f, 10, 8);
            m_PacMan->AddComponent(m_MoveComponent);
            auto pointsComponent = std::make_shared<PointsComponent>(m_PacMan);
            m_PacMan->AddComponent(pointsComponent);
            auto healthComponent = std::make_shared<HealthComponent>(m_PacMan);
            m_PacMan->AddComponent(healthComponent);
            auto colliderComponent = std::make_shared<BoxColliderComponent>(m_PacMan, 13.f, 13.f, glm::vec2(-8, -8));
            m_PacMan->AddComponent(colliderComponent);
            auto renderComponent = std::make_shared<RenderComponent>(m_PacMan, "pacman.png");
            m_PacMan->AddComponent(renderComponent);

            pointsComponent->GetPointSubject()->AddObserver(m_ScoreObserver);
            healthComponent->GetHealthSubject()->AddObserver(m_HealthObserver);

            //m_TransFormComponent->SetLocalPosition(100, 200, 0);
            m_Scene.Add(m_PacMan);
        }

        void SetupInput() {
            auto& input = dae::InputManager::GetInstance();

            // Set keyboard commands for movement
            input.SetKeyboardCommand(SDL_SCANCODE_S, new dae::ChangeMoveDirCommand(m_PacMan, PacManMoveComponent::Movement::DOWN, 90.f), dae::KeyState::keyDown);
            input.SetKeyboardCommand(SDL_SCANCODE_D, new dae::ChangeMoveDirCommand(m_PacMan, PacManMoveComponent::Movement::RIGHT, 0.f), dae::KeyState::keyDown);
            input.SetKeyboardCommand(SDL_SCANCODE_W, new dae::ChangeMoveDirCommand(m_PacMan, PacManMoveComponent::Movement::UP, 270.f), dae::KeyState::keyDown);
            input.SetKeyboardCommand(SDL_SCANCODE_A, new dae::ChangeMoveDirCommand(m_PacMan, PacManMoveComponent::Movement::LEFT, 180.f), dae::KeyState::keyDown);

            // Set keyboard commands for adding points and removing health
            input.SetKeyboardCommand(SDL_SCANCODE_Z, new dae::AddPointsCommand(m_PacMan, 10), dae::KeyState::keyDown);
            input.SetKeyboardCommand(SDL_SCANCODE_X, new dae::AddPointsCommand(m_PacMan, 100), dae::KeyState::keyDown);
            input.SetKeyboardCommand(SDL_SCANCODE_C, new dae::RemoveHealthCommand(m_PacMan), dae::KeyState::keyDown);
        }

    private:
        Scene& m_Scene;
        int m_InitialScore;
        int m_InitialHealth;
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
    };
}

