#include "HealthBoardComponent.h"
#include "HealthComponent.h"

dae::HealthBoardComponent::HealthBoardComponent(std::shared_ptr<GameObject> pOwner)
	: BaseComponent(pOwner)
{
	m_pText = pOwner.get()->GetComponent<TextComponent>();
}

void dae::HealthBoardComponent::Update(float)
{
}

void dae::HealthBoardComponent::Notify(GameObject& actor, Event events)
{
	switch (events)
	{
	case dae::Event::ActorDie:
	{
		m_pText.get()->SetText("# is dead:");
		break;
	}	
	case dae::Event::HealthRemoved:
	{
		int health = actor.GetComponent<HealthComponent>().get()->GetHealth();
		m_pText.get()->SetText("# lives: " + std::to_string(health));
		break;
	}
	}
}
