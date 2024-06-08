#include "CollectableComponent.h"
#include "ServiceLocator.h"

namespace dae
{
    int CollectableComponent::m_TotalCoins = 0;

    CollectableComponent::CollectableComponent(std::weak_ptr<GameObject> owner, CollectableInfo info)
        : BaseComponent(owner),
        m_PointsOnPickup(0),
        m_LoseHealthOnPickup(0),
        m_Info(info)
    {
        m_BoxCollider = GetOwner().lock()->GetComponent<BoxColliderComponent>();

        switch (m_Info.type)
        {
        case Object::COIN:
        case Object::POWERUP:
            m_PointsOnPickup = m_Info.value;
            break;
        case Object::GHOST:
            m_LoseHealthOnPickup = m_Info.value;
            break;
        case Object::NONE:
            break;
        }
    }

    void CollectableComponent::Update(float)
    {
        if (m_BoxCollider)
        {
            if (m_BoxCollider->GetOverlapped())
            {
                auto collidingObjects = m_BoxCollider->GetCollidingObjects();
                for (auto& object : collidingObjects)
                {
                    if (object.lock()->GetTag() == "Player")
                    {
                        OnPickup(object);
                    }
                }
            }
        }
    }

    void CollectableComponent::OnPickup(std::weak_ptr<GameObject> other)
    {
        if (auto otherGameObject = other.lock())
        {
            auto pointsComponent = otherGameObject->GetComponent<PointsComponent>();
            auto poweredUpComponent = otherGameObject->GetComponent<PoweredUpComponent>();
            auto healthComponent = otherGameObject->GetComponent<HealthComponent>();
            auto boxColliderComponent = otherGameObject->GetComponent<BoxColliderComponent>();

            switch (m_Info.type)
            {
            case Object::COIN:
                ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/munch_1.wav", 100, 0);
                if (pointsComponent)
                    pointsComponent->AddPoints(m_PointsOnPickup);
                GetOwner().lock()->RemoveObject();
                m_TotalCoins--;
                if (m_TotalCoins <= 0)
                {
                    auto& gameModeManager = GameModeManager::GetInstance();
                    if (gameModeManager.GetCurrentLevelGameMode() >= gameModeManager.GetAmountOfLevelsGameMode())
                        SceneManager::GetInstance().SetActiveScene("EndSceneWon");
                    GameModeManager::GetInstance().NextLevelActiveGameMode();
                }
                break;
            case Object::POWERUP:
                if (poweredUpComponent)
                {
                    ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/power_pellet.wav", 100, 1);
                    poweredUpComponent->SetPoweredUp();
                    poweredUpComponent->ResetTimer();
                }
                if (pointsComponent)
                    pointsComponent->AddPoints(m_PointsOnPickup);
                GetOwner().lock()->RemoveObject();
                break;
            case Object::GHOST:
                if (poweredUpComponent && poweredUpComponent->IsPoweredUp())
                {
                    ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/eat_ghost.wav", 100, 0);
                    if (pointsComponent)
                        pointsComponent->AddPoints(1000);
                    m_BoxCollider->SetActive(false);
                }
                else if (healthComponent)
                {
                    healthComponent->RemoveHealth(m_LoseHealthOnPickup);
                }
                break;
            default:
                break;
            }
        }
    }
}