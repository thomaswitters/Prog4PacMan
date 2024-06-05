#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class TransformComponent final : public BaseComponent
	{
	public:
		explicit TransformComponent(std::weak_ptr<GameObject> pOwner);
		void Update(float deltaTime) override;

		const void SetLocalPosition(float x, float y, float z);
		const void SetLocalPosition(const glm::vec3& pos);
		const void SetWorldPosition(const glm::vec3& pos);

		const void SetDirection(const glm::f32vec2& dir);
		const void SetAngle(const float& angle);

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();
		const float& GetRotation();
		const glm::f32vec2& GetDirection();

		void Translate(const glm::vec3& translation);
		void Rotate(float angle);

		void UpdateWorldPosition();
		void SetPositionDirty();
	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;
		glm::f32vec2 m_Direction;
		float m_RotationAngle;
		bool m_PositionIsDirty = true;
	};
}
