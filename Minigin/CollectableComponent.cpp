#include "CollectableComponent.h"

namespace dae
{
    CollectableComponent::CollectableComponent(std::weak_ptr<GameObject> owner, CollectableInfo info)
        : BaseComponent(owner),
        m_PointsOnPickup(0),
        m_LoseHealthOnPickup(0),
        m_Info(info)
    {
        m_BoxCollider = GetOwner().lock()->GetComponent<BoxColliderComponent>();

        switch (m_Info.object)
        {
        case Object::Coin:
            m_PointsOnPickup = m_Info.healthOrPoints;
            break;
        case Object::Ghost:
            m_LoseHealthOnPickup = m_Info.healthOrPoints;
            break;
        case Object::None:
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
                    OnPickup(object);
                }
            }
        }
    }

    void CollectableComponent::OnPickup(std::weak_ptr<GameObject> other)
    {

        if (auto otherGameObject = other.lock()) {
            switch (m_Info.object)
            {
            case Object::Coin:
            {
                auto pointsComponent = otherGameObject->GetComponent<PointsComponent>();

                if (pointsComponent)
                {
                    pointsComponent->AddPoints(m_PointsOnPickup);
                }

                GetOwner().lock()->RemoveObject();
                break;
            }
            case Object::Ghost:
            {
                auto healthComponent = otherGameObject->GetComponent<HealthComponent>();

                if (healthComponent)
                {
                    healthComponent->RemoveHealth(m_LoseHealthOnPickup);
                }

                GetOwner().lock()->RemoveObject();
                break;
            }
            default:
                // Add default case handling
                break;
            }
        }
    }
}