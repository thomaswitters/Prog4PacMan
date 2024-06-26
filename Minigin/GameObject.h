#pragma once
#include <memory>
#include "Transform.h"

//my code
#include <vector>
#include <iostream>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <chrono>
#include "TransformComponent.h"
#include <set>
#include "Scene.h"

namespace dae
{
    class Texture2D;

    class GameObject final : public std::enable_shared_from_this<GameObject>
    {
    public:
        void FixedUpdate();
        void Update(float deltaTime);
        void Render() const;

        GameObject() = default;
        virtual ~GameObject();
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        void AddComponent(std::shared_ptr<BaseComponent> component);
        void RemoveComponent(std::shared_ptr<BaseComponent> component);
        template <typename T>
        std::shared_ptr<T> GetComponent() const
        {
            for (std::shared_ptr<BaseComponent> bc : m_pComponents)
            {
                std::shared_ptr<T> castedPointer = std::dynamic_pointer_cast<T>(bc);
                if (castedPointer)
                {
                    return castedPointer;
                }
            }
            return nullptr;
        }
        bool HasComponent(const std::shared_ptr<BaseComponent>& component) const;

        std::weak_ptr<GameObject> GetParent() const { return m_pParent; };
        void SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition = false);
        std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_pChildren; }


        void SetTag(const std::string& tag) { m_Tag = tag; }
        std::string GetTag() const { return m_Tag; }
        bool HasTag(const std::string& tag) const;

        void SetScene(Scene* scene);
        Scene* GetScene() const;

        void RemoveObject();
        bool GetRemoveObject();

        void ClearComponents() { if (auto sharedThis = shared_from_this()) { m_pComponents.clear(); };}
    private:
        void RemoveChild(std::shared_ptr<GameObject> child);
        void AddChild(std::shared_ptr<GameObject> child);
        bool IsChild(const std::shared_ptr<GameObject>& parent) const;

        std::weak_ptr<GameObject> m_pParent;

        std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
        std::vector<std::shared_ptr<GameObject>> m_pChildren;

        Scene* m_pScene;
        std::string m_Tag;
        
        bool m_RemoveObject;
    };
}