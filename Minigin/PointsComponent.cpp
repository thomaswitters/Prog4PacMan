#include "PointsComponent.h"

dae::PointsComponent::PointsComponent(std::weak_ptr<GameObject> pOwner, int points)
	: BaseComponent(pOwner)
	, m_Points{ points }
{
	m_pPointsChanged = std::make_unique<Subject>();
}

void dae::PointsComponent::Update(float)
{
}

void dae::PointsComponent::AddPoints(int amount)
{
	m_Points += amount;
	m_pPointsChanged.get()->Notify(*GetOwner().lock(), Event::GainPoints);
}

int dae::PointsComponent::GetPoints() const
{
	return m_Points;
}

dae::Subject* dae::PointsComponent::GetPointSubject() const
{
	return m_pPointsChanged.get();
}
