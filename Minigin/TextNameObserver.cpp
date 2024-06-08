#include "TextNameObserver.h"
#include "PointsComponent.h"
#include "InputManager.h"

dae::TextNameObserver::TextNameObserver(std::weak_ptr<GameObject> pOwner)
{
	m_pText = pOwner.lock().get()->GetComponent<TextComponent>();
}

void dae::TextNameObserver::Notify(GameObject&, Event events)
{
	switch (events)
	{
	case dae::Event::TypeName:
		std::string name = dae::InputManager::GetInstance().GetTypedString();
		m_pText.get()->SetText("NAME: " + name);
		break;
	}
}