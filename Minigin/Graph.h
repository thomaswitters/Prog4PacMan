#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <glm/vec2.hpp>
#include "Renderer.h"

enum class CellType { Wall, Dot, Tunnel };

class Grid {
public:
    Grid(int numRows, int numCols, float cellWidth, float cellHeight, float x = 0.0f, float y = 0.0f);

    void SetCell(int row, int col, CellType type);
    CellType GetCell(int row, int col) const;
    int GetCellId(int row, int col) const;
    int GetCellId(const glm::vec2& position) const;
    std::pair<int, int> GetCellPosition(int cellId) const;
    std::pair<float, float> GetCellCenter(int cellId) const;
    void DrawGrid() const;
    void SetRowHeight(int row, float height);
    void SetColWidth(int col, float width);
    int GetNextCellId(int currentCellId, const glm::vec2& direction) const;
    int GetNextCellId5Front(int currentCellId, const glm::vec2& pacMandirection, const glm::vec2& ghostdirection) const;

    std::vector<int> FindPath(int startCellId, int goalCellId);
    std::vector<int> FindSecondBestPath(int startCellId, int goalCellId);
    std::vector<int> FindFarthestPath(int startCellId, int playerCellId);

private:
    int m_Rows;
    int m_Cols;
    float m_GridX;
    float m_GridY;
    std::vector<float> m_RowHeights;
    std::vector<float> m_ColWidths;
    std::vector<std::vector<int>> m_CellIds;
    std::vector<std::vector<CellType>> m_Cells;

    float Heuristic(const std::pair<int, int>& start, const std::pair<int, int>& goal) const;
    struct CompareNode {
        bool operator()(const std::pair<int, float>& n1, const std::pair<int, float>& n2);
    };
    std::vector<int> AStarSearch(int startCellId, int goalCellId, const std::unordered_set<int>& excludedNodes = {}) const;
};
