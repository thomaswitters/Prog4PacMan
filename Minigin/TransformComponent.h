#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(std::weak_ptr<GameObject> pOwner);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) = delete;
		TransformComponent& operator= (const TransformComponent&) = delete;
		TransformComponent& operator= (const TransformComponent&&) = delete;

		void Update(float deltaTime) override;
		const glm::vec3& GetPosition() const;
		void SetPosition(const float x, const float y, const float z);

	private:
		glm::vec3 m_Position;
	};
}
