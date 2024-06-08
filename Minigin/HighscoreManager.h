#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include "Singleton.h"
#include "GameObject.h"

namespace dae
{
    class HighscoreManager : public Singleton<HighscoreManager>
    {
    public:
        struct Score {
            std::string playerName;
            int score;
        };

        static bool CompareScores(const Score& a, const Score& b);
        void SetPlayerName(const std::string& playerName);
        std::string GetPlayerName() const;
        void SaveSoloHighscores(const std::vector<Score>& highscores);
        std::vector<Score> LoadSoloHighscores();
        void SaveCoopHighscores(const std::vector<Score>& highscores);
        std::vector<Score> LoadCoopHighscores();
        void SaveVersusHighscores(const std::vector<Score>& highscores);
        std::vector<Score> LoadVersusHighscores();
        void SetNameObject(std::shared_ptr<GameObject> object);
        std::shared_ptr<GameObject> GetNameObject();
    private:
        std::shared_ptr<GameObject> m_pNameObject;
        std::string m_PlayerName;

        void SaveHighscores(const std::vector<Score>& highscores, const std::string& filename);

        std::vector<Score> LoadHighscores(const std::string& filename);
    };
}

