#include "FPSComponent.h"
#include "TextComponent.h"
#include <SDL_ttf.h>
#include <chrono>

using namespace dae;


FPSComponent::FPSComponent(std::weak_ptr<GameObject> owner, const std::shared_ptr<Font>& font) :
	BaseComponent(owner),
	m_pTextComponent(std::make_unique<TextComponent>(owner, "FPS: 00", font, SDL_Color{ 255, 255, 255, 255 })),
	m_ElapsedTime(0.f),
	m_FrameCount(0),
	m_Fps(0.f),
	m_NeedsUpdate(true)
{}

void FPSComponent::Update(float deltaTime)
{
	m_pTextComponent->Update(deltaTime);
	UpdateFps(deltaTime);
}

void FPSComponent::Render() const
{
	m_pTextComponent->Render();
}

void FPSComponent::UpdateFps(float deltaTime)
{
	static float updateTimer = 0.0f;
	const float updateInterval = 0.2f; // Update interval in seconds

	updateTimer += deltaTime;

	m_ElapsedTime += deltaTime;
	m_FrameCount++;

	if (updateTimer >= updateInterval || m_NeedsUpdate)
	{
		m_Fps = static_cast<float>(m_FrameCount) / updateTimer;

		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << m_Fps;
		m_pTextComponent->SetText(stream.str() + " FPS");


		updateTimer = 0.0f;
		m_FrameCount = 0;

		m_NeedsUpdate = false;
	}
}
