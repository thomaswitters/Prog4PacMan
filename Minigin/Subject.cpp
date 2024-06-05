#include "Subject.h"

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	m_pObservers.push_back(observer);
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	m_pObservers.erase(std::find(m_pObservers.begin(), m_pObservers.end(), observer));
}

void dae::Subject::Notify(GameObject& actor, Event events)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		m_pObservers[i]->Notify(actor, events);
	}
}
