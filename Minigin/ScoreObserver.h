#pragma once
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class ScoreObserver final : public Observer
	{
	public:
		ScoreObserver(std::weak_ptr<GameObject> pOwner);
		~ScoreObserver() = default;

		ScoreObserver(const ScoreObserver&) = delete;
		ScoreObserver(ScoreObserver&&) = delete;
		ScoreObserver& operator= (const ScoreObserver&) = delete;
		ScoreObserver& operator= (const ScoreObserver&&) = delete;

		void Notify(GameObject& actor, Event events) override;
	private:
		std::shared_ptr<TextComponent> m_pText;
	};
}
