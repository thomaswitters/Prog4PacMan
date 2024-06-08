#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_SoundSysmService; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& system)
		{
			if (system == nullptr)
			{
				m_SoundSysmService = std::make_unique<dae::NullSoundSystem>();
			}
			m_SoundSysmService = std::move(system);
		}
	private:
		static std::unique_ptr<SoundSystem> m_SoundSysmService;
	};
}

