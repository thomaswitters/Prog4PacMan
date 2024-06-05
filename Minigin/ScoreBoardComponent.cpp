#include "ScoreBoardComponent.h"
#include "PointsComponent.h"

dae::ScoreBoardComponent::ScoreBoardComponent(std::shared_ptr<GameObject> pOwner)
	: BaseComponent(pOwner)
{
	m_pText = pOwner.get()->GetComponent<TextComponent>();
}

void dae::ScoreBoardComponent::Update(float)
{
}

void dae::ScoreBoardComponent::Notify(GameObject& actor, Event events)
{
	switch (events)
	{
	case dae::Event::GainPoints:
		int points = actor.GetComponent<PointsComponent>().get()->GetPoints();
		m_pText.get()->SetText("Score: " + std::to_string(points));
		break;
	}
}
