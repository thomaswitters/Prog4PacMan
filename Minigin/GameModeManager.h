#pragma once

#include <memory>
#include "Singleton.h"
#include "GameMode.h"

namespace dae
{
    class GameModeManager : public Singleton<GameModeManager>
    {
    public:
        void SetActiveGameMode(std::shared_ptr<GameMode> pGameMode);
        void SetupActiveGameMode();
        void NextLevelActiveGameMode();
        int GetAmountOfLevelsGameMode();
        int GetCurrentLevelGameMode();

    private:
        std::shared_ptr<GameMode> m_pActiveGameMode;
        friend class Singleton<GameModeManager>;
    };
}