#pragma once
#include <memory>
#include "BaseComponent.h"
#include "BoxColliderComponent.h"

namespace dae
{
    class CollectableComponent final : public BaseComponent
    {
    public:
        CollectableComponent(std::weak_ptr<GameObject> owner, int pointsOnPickup);

        CollectableComponent(const CollectableComponent&) = delete;
        CollectableComponent(CollectableComponent&&) = delete;
        CollectableComponent& operator= (const CollectableComponent&) = delete;
        CollectableComponent& operator= (const CollectableComponent&&) = delete;

        void Update(float) override;
        void OnPickup(std::weak_ptr<GameObject> other);
    private:
        int m_PointsOnPickup;
        std::shared_ptr<BoxColliderComponent> m_BoxCollider;
    };
}
