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

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();

		const void SetLocalPosition(float x, float y, float z);
		const void SetLocalPosition(const glm::vec3& pos);
		const void SetWorldPosition(const glm::vec3& pos);

		void UpdateWorldPosition();
		void SetPositionDirty();

	private:
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ true };
	};
}
