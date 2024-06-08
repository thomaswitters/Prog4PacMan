#pragma once
#include <memory>
#include "BaseComponent.h"
#include "BoxColliderComponent.h"
#include "PointsComponent.h"
#include "HealthComponent.h"
#include "PoweredUpComponent.h"
#include "Scene.h"
#include "GameModeManager.h"
#include "ServiceLocator.h"

namespace dae
{
    enum class Object
    {
        GHOST,
        COIN,
        POWERUP,
        NONE
    };

    struct CollectableInfo
    {
        Object type;
        int value;
    };

    class CollectableComponent final : public BaseComponent
    {
    public:
        CollectableComponent(std::weak_ptr<GameObject> owner, CollectableInfo info = { Object::NONE, 0 });

        CollectableComponent(const CollectableComponent&) = delete;
        CollectableComponent(CollectableComponent&&) = delete;
        CollectableComponent& operator=(const CollectableComponent&) = delete;
        CollectableComponent& operator=(const CollectableComponent&&) = delete;

        void Update(float deltaTime) override;
        void OnPickup(std::weak_ptr<GameObject> other);

        static void SetTotalCoins(int amount) { m_TotalCoins = amount; };

    private:
        static int m_TotalCoins;
        int m_PointsOnPickup;
        int m_LoseHealthOnPickup;
        std::shared_ptr<BoxColliderComponent> m_BoxCollider;
        CollectableInfo m_Info;
    };
}
