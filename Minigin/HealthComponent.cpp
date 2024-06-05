#include "HealthComponent.h"


dae::HealthComponent::HealthComponent(std::weak_ptr<GameObject> pOwner, int health)
	: BaseComponent(pOwner)
	, m_Health{ health }
	, m_pBoxColliderComponentComponent()
	, m_pMoveComponent()
	, m_Died(false)
{
	m_pHealthChanged = std::make_unique<Subject>();
	m_pBoxColliderComponentComponent = pOwner.lock().get()->GetComponent<BoxColliderComponent>();
	m_pMoveComponent = pOwner.lock().get()->GetComponent<PacManMoveComponent>();
}

void dae::HealthComponent::Update(float)
{

}

void dae::HealthComponent::RemoveHealth(int amount)
{
	if (m_Health > 0)
	{
		m_Health -= amount;
		m_pBoxColliderComponentComponent->SetActive(false);
		m_pMoveComponent->Respawn(m_pBoxColliderComponentComponent);
		m_Died = true;
		m_pHealthChanged.get()->Notify(*GetOwner().lock(), Event::HealthRemoved);
	}
	else
	{
		m_pHealthChanged.get()->Notify(*GetOwner().lock(), Event::ActorDie);
	}
}

int dae::HealthComponent::GetHealth() const
{
	return m_Health;
}

bool dae::HealthComponent::GetHasDied() const
{
	return m_Health;
}

dae::Subject* dae::HealthComponent::GetHealthSubject() const
{
	return m_pHealthChanged.get();
}
