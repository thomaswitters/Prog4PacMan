#pragma once
#include <memory>
#include "string"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
	class RotationComponent final : public BaseComponent
	{
	public:
		RotationComponent(std::weak_ptr<GameObject> pOwner, float rotateSpeed, bool rotateClockwise = true);
		virtual ~RotationComponent() = default;

		RotationComponent(const RotationComponent&) = delete;
		RotationComponent(RotationComponent&&) = delete;
		RotationComponent& operator= (const RotationComponent&) = delete;
		RotationComponent& operator= (const RotationComponent&&) = delete;

		void Update(float deltaTime) override;
	private:
		float m_Angle;
		float m_Radius;
		float m_RotationSpeed;

		bool m_RotateClockwise;

		glm::vec3 m_StartPos;

		bool m_HasStartPos;
	};
}

