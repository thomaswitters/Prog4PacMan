#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(std::weak_ptr<GameObject> owner, const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color);

		virtual ~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		std::string GetText() const { return m_Text; }

	private:
		std::string m_Text{};
		std::shared_ptr<Font> m_pFont{};
		SDL_Color m_Color{};
		std::shared_ptr<Texture2D> m_pTextTexture{};

		bool m_NeedsUpdate{ true };
	};
}

