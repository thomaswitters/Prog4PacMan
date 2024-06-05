#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class ScoreBoardComponent final : public BaseComponent, public Observer
	{
	public:
		ScoreBoardComponent(std::shared_ptr<GameObject> pOwner);
		~ScoreBoardComponent() = default;

		ScoreBoardComponent(const ScoreBoardComponent&) = delete;
		ScoreBoardComponent(ScoreBoardComponent&&) = delete;
		ScoreBoardComponent& operator= (const ScoreBoardComponent&) = delete;
		ScoreBoardComponent& operator= (const ScoreBoardComponent&&) = delete;

		void Update(float deltaTime) override;

		void Notify(GameObject& actor, Event events) override;
	private:
		std::shared_ptr<TextComponent> m_pText;
	};
}

