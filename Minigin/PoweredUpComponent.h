#pragma once
#include "BaseComponent.h"
#include "GameObject.h"

namespace dae
{
	class PoweredUpComponent : public BaseComponent
	{
	public:
		PoweredUpComponent(std::weak_ptr<GameObject> owner, float maxTime);

		void Update(float deltaTime) override;
		void Render() const override;

		void SetPoweredUp();
		void SetTimerPoweredUpStart(float deltaTime);
		void CheckEndTime();

		static bool IsPoweredUp() { return s_IsPoweredUp; };
	private:
		static bool s_IsPoweredUp;
		float m_MaxTimePoweredUp;
		static float s_TimerPoweredUp;
	};
}

