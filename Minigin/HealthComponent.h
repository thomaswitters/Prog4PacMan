#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(std::shared_ptr<GameObject> pOwner, int health = 3);
		~HealthComponent() = default;

		HealthComponent(const HealthComponent&) = delete;
		HealthComponent(HealthComponent&&) = delete;
		HealthComponent& operator= (const HealthComponent&) = delete;
		HealthComponent& operator= (const HealthComponent&&) = delete;

		void Update(float deltaTime) override;
		void RemoveHealth(int amount);

		int GetHealth();
		Subject* GetHealthSubject() const;

	private:
		std::unique_ptr<Subject> m_pHealthChanged;

		int m_Health;
	};
}

