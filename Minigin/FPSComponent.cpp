#include "FPSComponent.h"
#include "TextComponent.h"
#include <SDL_ttf.h>
#include <chrono>

using namespace dae;


FPSComponent::FPSComponent(std::weak_ptr<GameObject> owner) :
	BaseComponent(owner),
	m_ElapsedTime(0.f),
	m_FrameCount(0),
	m_Fps(0.f)
{

}

void FPSComponent::Update(float deltaTime)
{
	UpdateFps(deltaTime);
}

void FPSComponent::Render() const
{
	
}

void FPSComponent::UpdateFps(float deltaTime)
{
	const float updateInterval = 0.2f; // Update interval in seconds

	m_ElapsedTime += deltaTime;
	m_FrameCount++;

	if (m_ElapsedTime >= updateInterval)
	{
		float currentFps = static_cast<float>(m_FrameCount) / m_ElapsedTime;

		
		if (currentFps != m_Fps)
		{
			m_Fps = currentFps;

			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << m_Fps << " FPS";
			//m_pTextComponent->SetText(stream.str());


			auto textComponent = GetOwner().lock()->GetComponent<TextComponent>();
			if (textComponent)
			{
				textComponent->SetText(stream.str());
			}
		}

		m_ElapsedTime = 0.0f;
		m_FrameCount = 0;
	}
}
