#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace dae
{
    class BoxColliderComponent final : public BaseComponent
    {
    public:
        BoxColliderComponent(std::weak_ptr<GameObject> owner, float width, float height, const glm::vec2& offset = glm::vec2(0.0f, 0.0f));
        ~BoxColliderComponent() = default;

        BoxColliderComponent(const BoxColliderComponent&) = delete;
        BoxColliderComponent(BoxColliderComponent&&) = delete;
        BoxColliderComponent& operator=(const BoxColliderComponent&) = delete;
        BoxColliderComponent& operator=(BoxColliderComponent&&) = delete;

        void Update(float) override;
        void Render() const override;

        void ResolveCollision(const std::shared_ptr<BoxColliderComponent>& otherCollider);
        bool CheckCollision(const std::shared_ptr<BoxColliderComponent>& otherCollider) const;
        std::vector<std::weak_ptr<dae::GameObject>> GetCollidingObjects() const;
        glm::vec2 GetMinBounds() const;
        glm::vec2 GetMaxBounds() const;

        void SetTrigger(bool isStatic) { m_IsTrigger = isStatic; }
        bool IsTrigger() const { return m_IsTrigger; }

        bool GetOverlapped() { return m_IsOverlapped; }

        void FetchColliders();
    private:
        float m_Width;
        float m_Height;
        glm::vec2 m_Offset;
        Scene* m_Scene;

        bool m_IsTrigger;

        bool m_IsOverlapped;

        std::vector<std::weak_ptr<BoxColliderComponent>> m_Colliders;
        static std::vector<std::weak_ptr<GameObject>> CollidableObjects;

        std::shared_ptr<TransformComponent> m_TransformComponent;
    };

   
}