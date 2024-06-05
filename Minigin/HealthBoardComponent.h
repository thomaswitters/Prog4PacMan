#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class HealthBoardComponent final : public BaseComponent, public Observer
	{
	public:
		HealthBoardComponent(std::shared_ptr<GameObject> pOwner);
		~HealthBoardComponent() = default;

		HealthBoardComponent(const HealthBoardComponent&) = delete;
		HealthBoardComponent(HealthBoardComponent&&) = delete;
		HealthBoardComponent& operator= (const HealthBoardComponent&) = delete;
		HealthBoardComponent& operator= (const HealthBoardComponent&&) = delete;

		void Update(float deltaTime) override;

		void Notify(GameObject& actor, Event events) override;
	private:
		std::shared_ptr<TextComponent> m_pText;
	};
}

