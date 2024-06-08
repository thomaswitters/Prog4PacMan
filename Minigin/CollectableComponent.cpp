#include "CollectableComponent.h"
#include "ServiceLocator.h"

namespace dae
{
    int CollectableComponent::m_TotalCoins = 0;

    CollectableComponent::CollectableComponent(std::weak_ptr<GameObject> pOwner, CollectableInfo info)
        : BaseComponent(pOwner),
        m_PointsOnPickup(0),
        m_LoseHealthOnPickup(0),
        m_Info(info),
        m_PointsOnPickupGhost(200)
    {
        m_pBoxCollider = GetOwner().lock()->GetComponent<BoxColliderComponent>();

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
        if (m_pBoxCollider)
        {
            if (m_pBoxCollider->GetOverlapped())
            {
                auto collidingObjects = m_pBoxCollider->GetCollidingObjects();
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

    void CollectableComponent::OnPickup(std::weak_ptr<GameObject> pOther)
    {
        if (auto pOtherGameObject = pOther.lock())
        {
            auto pPointsComponent = pOtherGameObject->GetComponent<PointsComponent>();
            auto pPoweredUpComponent = pOtherGameObject->GetComponent<PoweredUpComponent>();
            auto pHealthComponent = pOtherGameObject->GetComponent<HealthComponent>();
            auto pBoxColliderComponent = pOtherGameObject->GetComponent<BoxColliderComponent>();

            switch (m_Info.type)
            {
            case Object::COIN:
                ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/munch_1.wav", 100, 0);
                if (pPointsComponent)
                    pPointsComponent->AddPoints(m_PointsOnPickup);
                GetOwner().lock()->RemoveObject();
                m_TotalCoins--;
                if (m_TotalCoins <= 0)
                {
                    auto& gameModeManager = GameModeManager::GetInstance();
                    gameModeManager.NextLevelActiveGameMode();
                }
                break;
            case Object::POWERUP:
                if (pPoweredUpComponent)
                {
                    ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/power_pellet.wav", 100, 1);
                    pPoweredUpComponent->SetPoweredUp();
                    pPoweredUpComponent->ResetTimer();
                }
                if (pPointsComponent)
                    pPointsComponent->AddPoints(m_PointsOnPickup);
                GetOwner().lock()->RemoveObject();
                break;
            case Object::GHOST:
                if (pPoweredUpComponent && pPoweredUpComponent->IsPoweredUp())
                {
                    ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/eat_ghost.wav", 100, 0);
                    if (pPointsComponent)
                        pPointsComponent->AddPoints(m_PointsOnPickupGhost);
                    m_pBoxCollider->SetActive(false);
                }
                else if (pHealthComponent)
                {
                    pHealthComponent->RemoveHealth(m_LoseHealthOnPickup);
                }
                break;
            default:
                break;
            }
        }
    }
}