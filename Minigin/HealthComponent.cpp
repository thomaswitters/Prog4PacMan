#include "HealthComponent.h"


dae::HealthComponent::HealthComponent(std::weak_ptr<GameObject> pOwner, int health)
	: BaseComponent(pOwner)
	, m_Health{ health }
{
	m_pHealthChanged = std::make_unique<Subject>();
}

void dae::HealthComponent::Update(float)
{

}

void dae::HealthComponent::RemoveHealth(int amount)
{
	if (m_Health > 0)
	{
		m_Health -= amount;
		m_pHealthChanged.get()->Notify(*GetOwner().lock(), Event::HealthRemoved);
	}
	else
	{
		m_pHealthChanged.get()->Notify(*GetOwner().lock(), Event::ActorDie);
	}
}

int dae::HealthComponent::GetHealth()
{
	return m_Health;
}

dae::Subject* dae::HealthComponent::GetHealthSubject() const
{
	return m_pHealthChanged.get();
}
