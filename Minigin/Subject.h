#pragma once
#pragma once
#include <memory>
#include <vector>
#include "Observer.h"
#include "GameObject.h"

namespace dae
{
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		Subject(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator= (const Subject&) = delete;
		Subject& operator= (const Subject&&) = delete;

		void AddObserver(std::shared_ptr<Observer> observer);
		void RemoveObserver(std::shared_ptr<Observer> observer);
		void Notify(GameObject& actor, Event events);

	private:
		std::vector<std::shared_ptr<Observer>> m_pObservers;
		int m_NumObservers{ 0 };
		const int m_MaxNumObservers{ 10 };
	};
}
