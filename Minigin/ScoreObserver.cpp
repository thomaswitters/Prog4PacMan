#include "ScoreObserver.h"
#include "PointsComponent.h"

dae::ScoreObserver::ScoreObserver(std::weak_ptr<GameObject> pOwner)
{
	m_pText = pOwner.lock().get()->GetComponent<TextComponent>();
}

void dae::ScoreObserver::Notify(GameObject& actor, Event events)
{
	switch (events)
	{
	case dae::Event::GainPoints:
		int points = actor.GetComponent<PointsComponent>().get()->GetPoints();
		m_pText.get()->SetText("SCORE: " + std::to_string(points));
		break;
	}
}