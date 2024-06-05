#pragma once
#include "Observer.h"
#include "TextComponent.h"
#include "PacManMoveComponent.h"
#include "BoxColliderComponent.h"

namespace dae
{
	class HealthObserver final : public Observer
	{
	public:
		HealthObserver(std::weak_ptr<GameObject> pOwner);
		~HealthObserver() = default;

		HealthObserver(const HealthObserver&) = delete;
		HealthObserver(HealthObserver&&) = delete;
		HealthObserver& operator= (const HealthObserver&) = delete;
		HealthObserver& operator= (const HealthObserver&&) = delete;

		void Notify(GameObject& actor, Event events) override;
	private:
		std::shared_ptr<TextComponent> m_pText;
	};
}
