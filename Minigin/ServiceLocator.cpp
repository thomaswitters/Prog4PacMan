#include "ServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_SoundSysmService{ std::make_unique<dae::NullSoundSystem>() };