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

    void setCell(int row, int col, CellType type);
    CellType getCell(int row, int col) const;
    int getCellId(int row, int col) const;
    int getCellId(const glm::vec2& position) const;
    std::pair<int, int> getCellPosition(int cellId) const;
    std::pair<float, float> getCellCenter(int cellId) const;
    void drawGrid() const;
    void setRowHeight(int row, float height);
    void setColWidth(int col, float width);
    int getNextCellId(int currentCellId, const glm::vec2& direction) const;
    int getNextCellId5Front(int currentCellId, const glm::vec2& pacMandirection, const glm::vec2& ghostdirection) const;

    std::vector<int> FindPath(int startCellId, int goalCellId);
    std::vector<int> FindSecondBestPath(int startCellId, int goalCellId);
    std::vector<int> FindFarthestPath(int startCellId, int playerCellId);

private:
    int rows;
    int cols;
    float gridX;
    float gridY;
    std::vector<float> rowHeights;
    std::vector<float> colWidths;
    std::vector<std::vector<int>> cellIds;
    std::vector<std::vector<CellType>> cells;

    float Heuristic(const std::pair<int, int>& start, const std::pair<int, int>& goal) const;
    struct CompareNode {
        bool operator()(const std::pair<int, float>& n1, const std::pair<int, float>& n2);
    };
    std::vector<int> AStarSearch(int startCellId, int goalCellId, const std::unordered_set<int>& excludedNodes = {}) const;
};