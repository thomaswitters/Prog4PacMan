#pragma once
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class TextNameObserver final : public Observer
	{
	public:
		TextNameObserver(std::weak_ptr<GameObject> pOwner);
		~TextNameObserver() = default;

		TextNameObserver(const TextNameObserver&) = delete;
		TextNameObserver(TextNameObserver&&) = delete;
		TextNameObserver& operator= (const TextNameObserver&) = delete;
		TextNameObserver& operator= (const TextNameObserver&&) = delete;

		void Notify(GameObject& actor, Event events) override;
	private:
		std::shared_ptr<TextComponent> m_pText;
	};
}

