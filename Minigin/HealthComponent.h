#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include "PacManMoveComponent.h"
#include "BoxColliderComponent.h"

namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(std::weak_ptr<GameObject> pOwner, int health = 3);
		~HealthComponent() = default;

		HealthComponent(const HealthComponent&) = delete;
		HealthComponent(HealthComponent&&) = delete;
		HealthComponent& operator= (const HealthComponent&) = delete;
		HealthComponent& operator= (const HealthComponent&&) = delete;

		void Update(float deltaTime) override;
		void RemoveHealth(int amount);

		int GetHealth() const;
		bool GetHasDied() const;
		Subject* GetHealthSubject() const;
	private:
		std::unique_ptr<Subject> m_pHealthChanged;
		std::shared_ptr<PacManMoveComponent> m_pMoveComponent;
		std::shared_ptr<BoxColliderComponent> m_pBoxColliderComponentComponent;
		int m_Health;
		bool m_Died;
	};
}

