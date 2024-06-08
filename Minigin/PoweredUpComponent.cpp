#include "PoweredUpComponent.h"
#include "ServiceLocator.h"

using namespace dae;

bool PoweredUpComponent::s_IsPoweredUp = false;
float PoweredUpComponent::s_TimerPoweredUp = false;

PoweredUpComponent::PoweredUpComponent(std::weak_ptr<GameObject> owner, float maxTime) :
	BaseComponent(owner),
	m_MaxTimePoweredUp(maxTime)
{
}

void PoweredUpComponent::Update(float deltaTime)
{
	if (s_IsPoweredUp)
	{
		SetTimerPoweredUpStart(deltaTime);
		CheckEndTime();
	}
}

void PoweredUpComponent::Render() const
{

}

void PoweredUpComponent::SetPoweredUp()
{
	s_IsPoweredUp = true;
}

void PoweredUpComponent::SetTimerPoweredUpStart(float deltaTime)
{
	if (s_TimerPoweredUp <= m_MaxTimePoweredUp)
	{
		s_TimerPoweredUp += deltaTime;
	}
}

void PoweredUpComponent::CheckEndTime()
{
	if (s_TimerPoweredUp >= m_MaxTimePoweredUp)
	{
		s_TimerPoweredUp = 0.f;
		s_IsPoweredUp = false;
	}
}