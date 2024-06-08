#include "HighscoreManager.h"

namespace dae
{
    bool HighscoreManager::CompareScores(const Score& a, const Score& b) {
        return a.score > b.score;
    }

    void HighscoreManager::SetPlayerName(const std::string& playerName) {
        if (playerName.empty())
        {
            m_PlayerName = "Player";
        }
        else
        {
            m_PlayerName = playerName;
        }
    }

    std::string HighscoreManager::GetPlayerName() const {
        return m_PlayerName;
    }

    void HighscoreManager::SaveSoloHighscores(const std::vector<Score>& highscores) {
        SaveHighscores(highscores, "solo_highscores.txt");
    }

    std::vector<HighscoreManager::Score> HighscoreManager::LoadSoloHighscores() {
        return LoadHighscores("solo_highscores.txt");
    }

    void HighscoreManager::SaveCoopHighscores(const std::vector<Score>& highscores) {
        SaveHighscores(highscores, "coop_highscores.txt");
    }

    std::vector<HighscoreManager::Score> HighscoreManager::LoadCoopHighscores() {
        return LoadHighscores("coop_highscores.txt");
    }

    void HighscoreManager::SaveVersusHighscores(const std::vector<Score>& highscores) {
        SaveHighscores(highscores, "versus_highscores.txt");
    }

    std::vector<HighscoreManager::Score> HighscoreManager::LoadVersusHighscores() {
        return LoadHighscores("versus_highscores.txt");
    }

    void HighscoreManager::SetNameObject(std::shared_ptr<GameObject> object) {
        m_pNameObject = object;
    }

    std::shared_ptr<GameObject> HighscoreManager::GetNameObject() {
        return m_pNameObject;
    }

    void HighscoreManager::SaveHighscores(const std::vector<Score>& highscores, const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const Score& score : highscores) {
                file << score.playerName << " " << score.score << std::endl;
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for writing!\n";
        }
    }

    std::vector<HighscoreManager::Score> HighscoreManager::LoadHighscores(const std::string& filename) {
        std::vector<Score> highscores;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string playerName;
            int score;
            while (file >> playerName >> score) {
                highscores.push_back({ playerName, score });
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for reading!\n";
        }
        return highscores;
    }
}
