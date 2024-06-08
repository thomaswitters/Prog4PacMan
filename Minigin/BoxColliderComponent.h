#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"

namespace dae
{
    class BoxColliderComponent final : public BaseComponent {
    public:
        BoxColliderComponent(std::weak_ptr<GameObject> owner, float width, float height, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));

        BoxColliderComponent(const BoxColliderComponent&) = delete;
        BoxColliderComponent(BoxColliderComponent&&) = delete;
        BoxColliderComponent& operator=(const BoxColliderComponent&) = delete;
        BoxColliderComponent& operator=(BoxColliderComponent&&) = delete;

        void Update(float deltaTime) override;
        void Render() const override;

        bool CheckCollision(const std::shared_ptr<BoxColliderComponent>& otherCollider) const;
        std::vector<std::weak_ptr<GameObject>> GetCollidingObjects() const;

        bool IsTrigger() const { return m_IsTrigger; }
        bool IsActive() const { return m_IsActive; }
        void SetActive(bool active) { m_IsActive = active; }
        bool GetOverlapped() const { return m_IsOverlapped; }

    private:
        glm::vec2 GetMinBounds() const;
        glm::vec2 GetMaxBounds() const;
        void FetchColliders();

        float m_Width;
        float m_Height;
        glm::vec2 m_Offset;

        bool m_IsTrigger;
        bool m_IsActive;
        bool m_IsOverlapped;

        std::vector<std::weak_ptr<BoxColliderComponent>> m_Colliders;
        static std::vector<std::weak_ptr<GameObject>> CollidableObjects;

        std::shared_ptr<TransformComponent> m_TransformComponent;
    };
}
