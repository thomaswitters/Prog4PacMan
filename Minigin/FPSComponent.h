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
		FPSComponent(std::weak_ptr<GameObject> owner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void UpdateFps(float deltaTime);

	private:
		int m_FrameCount;
		float m_ElapsedTime;
		float m_Fps;

		std::shared_ptr<TextComponent> m_TextComponent;
	};
}

