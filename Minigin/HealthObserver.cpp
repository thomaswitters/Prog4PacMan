#include "HealthObserver.h"
#include "HealthComponent.h"
#include "ServiceLocator.h"

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
		dae::SceneManager::GetInstance();
		dae::SceneManager::GetInstance().SetActiveScene("EndSceneLost");
		dae::ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/death_1.wav", 100, 0);
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
		dae::ServiceLocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/death_2.wav", 100, 0);
		int health = actor.GetComponent<HealthComponent>().get()->GetHealth();
		m_pText.get()->SetText("# LIVES: " + std::to_string(health));
		break;
	}
	}
}