#include "GameModeManager.h"

namespace dae
{
    void GameModeManager::SetActiveGameMode(std::shared_ptr<GameMode> pGameMode)
    {
        m_pActiveGameMode = pGameMode;
    }

    void GameModeManager::SetupActiveGameMode()
    {
        if (m_pActiveGameMode)
        {
            m_pActiveGameMode->SetupGameMode();
        }
    }

    void GameModeManager::NextLevelActiveGameMode()
    {
        if (m_pActiveGameMode)
        {
            m_pActiveGameMode->NextLevel();
        }
    }

    int GameModeManager::GetAmountOfLevelsGameMode()
    {
        if (m_pActiveGameMode)
        {
            return m_pActiveGameMode->GetAmountOfLevels();
        }
        return -1;
    }

    int GameModeManager::GetCurrentLevelGameMode()
    {
        if (m_pActiveGameMode)
        {
            return m_pActiveGameMode->GetCurrentLevel();
        }
        return -1;
    }
}
