#include "CollectableComponent.h"

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

        switch (m_Info.object)
        {
        case Object::COIN:
        case Object::POWERUP:
            m_PointsOnPickup = m_Info.healthOrPoints;
            break;
        case Object::GHOST:
            m_LoseHealthOnPickup = m_Info.healthOrPoints;
            break;
        case Object::NONE:
            break;
        }
    }

    void dae::CollectableComponent::Update(float)
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

            switch (m_Info.object)
            {
            case Object::COIN:
                if (pointsComponent)
                {
                    pointsComponent->AddPoints(m_PointsOnPickup);
                }
                GetOwner().lock()->RemoveObject();

                m_TotalCoins--;
                if (m_TotalCoins <= 0)
                {
                    dae::SceneManager::GetInstance().SetActiveScene("EndSceneWon");
                }
                break;

            case Object::POWERUP:
                if (poweredUpComponent)
                {
                    poweredUpComponent->SetPoweredUp();
                    
                }
                if (pointsComponent)
                {
                    pointsComponent->AddPoints(m_PointsOnPickup);
                }
                GetOwner().lock()->RemoveObject();
                break;

            case Object::GHOST:
                if (poweredUpComponent && poweredUpComponent->IsPoweredUp())
                {
                    if (pointsComponent)
                    {
                        pointsComponent->AddPoints(1000);
                    }
                    m_BoxCollider->SetActive(false);
                }
                else if (healthComponent)
                {
                    
                    healthComponent->RemoveHealth(m_LoseHealthOnPickup);
                    //boxColliderComponent->SetActive(false);
                }
                break;

            default:
                break;
            }
        }
    }
}