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

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		virtual void FixedUpdate();
		virtual void Update(float deltaTime);
		virtual void Render() const;


		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		glm::vec2 GetPosition();
		
		void AddComponent(std::shared_ptr<BaseComponent> component);
		void RemoveComponent(std::shared_ptr<BaseComponent> component);
		template <typename T>
		std::shared_ptr<T> GetComponent() const
		{
			for (std::shared_ptr<BaseComponent> bc : m_Components)
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

	private:
		Transform m_Transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_pTexture{};

		std::vector<std::shared_ptr<BaseComponent>> m_Components;
	};
}

