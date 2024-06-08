#include "HealthComponent.h"

namespace dae
{
	HealthComponent::HealthComponent(std::weak_ptr<GameObject> pOwner, int health)
		: BaseComponent(pOwner)
		, m_Health(health)
		, m_Died(false)
	{
		m_pHealthChanged = std::make_unique<Subject>();
		auto owner = pOwner.lock();
		if (owner)
		{
			m_pBoxColliderComponent = owner->GetComponent<BoxColliderComponent>();
			m_pMoveComponent = owner->GetComponent<PacManMoveComponent>();
		}
	}

	void HealthComponent::RemoveHealth(int amount)
	{
		if (m_Health > 0)
		{
			m_Health -= amount;
			if (m_pBoxColliderComponent)
				m_pBoxColliderComponent->SetActive(false);
			if (m_pMoveComponent)
				m_pMoveComponent->Respawn(m_pBoxColliderComponent);
			m_Died = true;
			if (m_pHealthChanged)
				m_pHealthChanged->Notify(*GetOwner().lock(), Event::HealthRemoved);
		}
		else if (m_pHealthChanged)
		{
			m_pHealthChanged->Notify(*GetOwner().lock(), Event::ActorDie);
		}
	}

	int HealthComponent::GetHealth() const
	{
		return m_Health;
	}

	bool HealthComponent::HasDied() const
	{
		return m_Died;
	}

	Subject* HealthComponent::GetHealthSubject() const
	{
		return m_pHealthChanged.get();
	}
}