#pragma once
#include <memory>
#include "BaseComponent.h"
#include "BoxColliderComponent.h"
#include "PointsComponent.h"
#include "HealthComponent.h"

namespace dae
{
    enum class Object
    {
        Ghost,
        Coin,
        None
    };

    struct CollectableInfo
    {
        Object object;
        int healthOrPoints;
    };

    class CollectableComponent final : public BaseComponent
    {
    public:
        CollectableComponent(std::weak_ptr<GameObject> owner, CollectableInfo info = {Object::None, 0});

        CollectableComponent(const CollectableComponent&) = delete;
        CollectableComponent(CollectableComponent&&) = delete;
        CollectableComponent& operator= (const CollectableComponent&) = delete;
        CollectableComponent& operator= (const CollectableComponent&&) = delete;

        void Update(float) override;
        void OnPickup(std::weak_ptr<GameObject> other);
    private:
        int m_PointsOnPickup;
        int m_LoseHealthOnPickup;
        std::shared_ptr<BoxColliderComponent> m_BoxCollider;

        CollectableInfo m_Info;
    };
}
