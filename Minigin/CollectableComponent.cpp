#include "CollectableComponent.h"
#include "ScoreObserver.h"
#include "PointsComponent.h"

namespace dae
{
    CollectableComponent::CollectableComponent(std::weak_ptr<GameObject> owner, int pointsOnPickup)
        : BaseComponent(owner), 
        m_PointsOnPickup(pointsOnPickup)
    {
        m_BoxCollider = GetOwner().lock()->GetComponent<BoxColliderComponent>();
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
        auto pointsComponent = other.lock()->GetComponent<PointsComponent>();
        //auto renderComponent = other.lock()->GetComponent<RenderComponent>();

        if (pointsComponent)
        {
            pointsComponent->AddPoints(m_PointsOnPickup);
        }

        GetOwner().lock()->RemoveObject();
    }
}