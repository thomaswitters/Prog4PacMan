#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_pSoundSysmLocator; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& system)
		{
			m_pSoundSysmLocator = std::move(system);
		}
	private:
		static std::unique_ptr<SoundSystem> m_pSoundSysmLocator;
	};
}

