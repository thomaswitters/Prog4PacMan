#include "Subject.h"

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	if (m_NumObservers <= m_MaxNumObservers)
	{
		m_pObservers.push_back(observer);
		++m_NumObservers;
	}
	else
	{
		throw std::length_error(std::string("Max number of observers"));
	}
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	if (m_NumObservers > 0)
	{
		m_pObservers.erase(std::find(m_pObservers.begin(), m_pObservers.end(), observer));
		--m_NumObservers;
	}
	else
	{
		throw std::length_error(std::string("There are no observers to remove"));
	}
}

void dae::Subject::Notify(GameObject& actor, Event events)
{
	for (int i = 0; i < m_NumObservers; i++)
	{
		m_pObservers[i]->Notify(actor, events);
	}
}
