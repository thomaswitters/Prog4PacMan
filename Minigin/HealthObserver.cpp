#include "HealthObserver.h"
#include "HealthComponent.h"

dae::HealthObserver::HealthObserver(std::weak_ptr<GameObject> pOwner)
{
	m_pText = pOwner.lock().get()->GetComponent<TextComponent>();
}


void dae::HealthObserver::Notify(GameObject& actor, Event events)
{
	switch (events)
	{
	case dae::Event::ActorDie:
	{
		std::string text = actor.GetTag();
		if (actor.HasTag("Player"))
		{
			m_pText.get()->SetText("# " + text + " is dead");
		}
		else
		{
			m_pText.get()->SetText("# is dead");
		}

		actor.RemoveObject();
		
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