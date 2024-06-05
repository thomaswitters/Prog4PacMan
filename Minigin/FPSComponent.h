#pragma once
#include "BaseComponent.h"
#include <chrono>
#include "Transform.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL_ttf.h>
#include "TransformComponent.h"

namespace dae
{
	class TextComponent;
	class Font;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(std::weak_ptr<GameObject> owner, const std::shared_ptr<Font>& font);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void UpdateFps(float deltaTime);

	private:
		std::unique_ptr<TextComponent> m_pTextComponent;

		bool m_NeedsUpdate;
		int m_FrameCount;
		float m_ElapsedTime;
		float m_Fps;
	};
}

