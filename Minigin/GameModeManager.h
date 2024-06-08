#pragma once

#include <memory>
#include "Singleton.h"
#include "GameMode.h"

namespace dae
{
    class GameModeManager : public Singleton<GameModeManager>
    {
    public:
        void SetActiveGameMode(std::shared_ptr<GameMode> gameMode)
        {
            m_ActiveGameMode = gameMode;
        }

        void SetupActiveGameMode()
        {
            if (m_ActiveGameMode)
            {
                m_ActiveGameMode->SetupGameMode();
            }
        }

        void NextLevelActiveGameMode()
        {
            if (m_ActiveGameMode)
            {
                m_ActiveGameMode->NextLevel();
            }
        }
        int GetAmountOfLevelsGameMode()
        {
            if (m_ActiveGameMode)
            {
                return m_ActiveGameMode->GetAmountOfLevels();
            }
            return -1;
        }
        int GetCurrentLevelGameMode()
        {
            if (m_ActiveGameMode)
            {
                return m_ActiveGameMode->GetCurrentLevel();
            }
            return -1;
        }
    private:
        std::shared_ptr<GameMode> m_ActiveGameMode;
        friend class Singleton<GameModeManager>;
    };
}
