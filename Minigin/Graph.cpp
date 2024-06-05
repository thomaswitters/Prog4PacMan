#include "Graph.h"
#include <unordered_set>
#include <glm/geometric.hpp>

Grid::Grid(int numRows, int numCols, float cellWidth, float cellHeight, float x, float y)
    : rows(numRows), cols(numCols), gridX(x), gridY(y) {
    rowHeights = std::vector<float>(rows, cellHeight);
    colWidths = std::vector<float>(cols, cellWidth);
    cells = std::vector<std::vector<CellType>>(rows, std::vector<CellType>(cols, CellType::Tunnel));
    cellIds.resize(rows, std::vector<int>(cols, 0));
    int id = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cellIds[i][j] = id++;
        }
    }
}

void Grid::setCell(int row, int col, CellType type) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        cells[row][col] = type;
    }
}

CellType Grid::getCell(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return cells[row][col];
    }
    return CellType::Tunnel;
}

int Grid::getCellId(int row, int col) const
{
    float cumulativeHeight = gridY;
    for (int i = 0; i < rows; ++i) {
        if (col >= cumulativeHeight && col < cumulativeHeight + rowHeights[i]) {
            row = i;
            break;
        }
        cumulativeHeight += rowHeights[i];
    }

    float cumulativeWidth = gridX;
    for (int j = 0; j < cols; ++j) {
        if (row >= cumulativeWidth && row < cumulativeWidth + colWidths[j]) {
            col = j;
            break;
        }
        cumulativeWidth += colWidths[j];
    }

    // If both row and column indices are valid, return the corresponding cell ID
    if (row != -1 && col != -1) {
        return cellIds[row][col];
    }
    return -1;
}

int Grid::getCellId(const glm::vec2& position) const {
    float localX = position.x - gridX;
    float localY = position.y - gridY;

    float accumulatedWidth = 0.0f;
    int col = -1;
    for (int i = 0; i < cols; ++i) {
        accumulatedWidth += colWidths[i];
        if (localX < accumulatedWidth) {
            col = i;
            break;
        }
    }

    float accumulatedHeight = 0.0f;
    int row = -1;
    for (int i = 0; i < rows; ++i) {
        accumulatedHeight += rowHeights[i];
        if (localY < accumulatedHeight) {
            row = i;
            break;
        }
    }

    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return getCellId(row, col);
    }
    else {
        return -1;
    }
}

std::pair<int, int> Grid::getCellPosition(int cellId) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cellIds[i][j] == cellId) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

std::pair<float, float> Grid::getCellCenter(int cellId) const {
    auto position = getCellPosition(cellId);
    if (position.first != -1 && position.second != -1) {
        float centerX = gridX;
        for (int j = 0; j < position.second; ++j) {
            centerX += colWidths[j];
        }
        centerX += colWidths[position.second] / 2;

        float centerY = gridY;
        for (int i = 0; i < position.first; ++i) {
            centerY += rowHeights[i];
        }
        centerY += rowHeights[position.first] / 2;

        return std::make_pair(centerX, centerY);
    }
    return std::make_pair(-1.0f, -1.0f);
}

void Grid::drawGrid() const {
    float currentY = gridY;
    for (int i = 0; i < rows; ++i) {
        float currentX = gridX;
        for (int j = 0; j < cols; ++j) {
            float cellWidth = colWidths[j];
            float cellHeight = rowHeights[i];

            switch (cells[i][j]) {
            case CellType::Tunnel:
                dae::Renderer::GetInstance().DrawRectangle(currentX, currentY, cellWidth, cellHeight, { 255, 255, 255, 0 });
                break;
            case CellType::Wall:
                dae::Renderer::GetInstance().DrawRectangle(currentX, currentY, cellWidth, cellHeight, { 0, 0, 0, 0 });
                break;
            }

            dae::Renderer::GetInstance().DrawRectangle(currentX, currentY, cellWidth, 1.0f, { 0, 0, 0, 255 });
            dae::Renderer::GetInstance().DrawRectangle(currentX + cellWidth - 1.0f, currentY, 1.0f, cellHeight, { 0, 0, 0, 255 });
            dae::Renderer::GetInstance().DrawRectangle(currentX, currentY + cellHeight - 1.0f, cellWidth, 1.0f, { 0, 0, 0, 255 });
            dae::Renderer::GetInstance().DrawRectangle(currentX, currentY, 1.0f, cellHeight, { 0, 0, 0, 255 });

            currentX += cellWidth;
        }
        currentY += rowHeights[i];
    }
}

void Grid::setRowHeight(int row, float height) {
    if (row >= 0 && row < rows) {
        rowHeights[row] = height;
    }
}

void Grid::setColWidth(int col, float width) {
    if (col >= 0 && col < cols) {
        colWidths[col] = width;
    }
}

int Grid::getNextCellId(int currentCellId, const glm::vec2& direction) const {
    int currentRow = currentCellId / cols;
    int currentCol = currentCellId % cols;

    int nextRow = currentRow + static_cast<int>(direction.y);
    int nextCol = currentCol + static_cast<int>(direction.x);

    if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
        return currentCellId;
    }

    return nextRow * cols + nextCol;
}

int Grid::getNextCellId5Front(int currentCellId, const glm::vec2& pacMandirection, const glm::vec2& ghostdirection) const {
    int currentRow = currentCellId / cols;
    int currentCol = currentCellId % cols;
    int lastValidRow = currentRow;
    int lastValidCol = currentCol;

    for (int i = 0; i < 4; ++i) {
        int nextRow = currentRow + static_cast<int>(pacMandirection.y) * (i + 1);
        int nextCol = currentCol + static_cast<int>(pacMandirection.x) * (i + 1);

        if (nextRow >= 0 && nextRow < rows && nextCol >= 0 && nextCol < cols) {
            glm::vec2 nextCellDirection = glm::vec2(nextCol - currentCol, nextRow - currentRow);
            if (glm::dot(nextCellDirection, ghostdirection) == -1) {
                return lastValidRow * cols + lastValidCol;
            }

            if (cells[nextRow][nextCol] == CellType::Wall) {
                return lastValidRow * cols + lastValidCol;
            }
            lastValidRow = nextRow;
            lastValidCol = nextCol;
        }
        else {
            return lastValidRow * cols + lastValidCol;
        }
    }
    return lastValidRow * cols + lastValidCol;
}

float Grid::Heuristic(const std::pair<int, int>& start, const std::pair<int, int>& goal) const {
    return static_cast<float>(std::abs(start.first - goal.first) + std::abs(start.second - goal.second));
}

bool Grid::CompareNode::operator()(const std::pair<int, float>& n1, const std::pair<int, float>& n2) {
    return n1.second > n2.second;
}

std::vector<int> Grid::AStarSearch(int startCellId, int goalCellId, const std::unordered_set<int>& excludedNodes) const {
    auto start = getCellPosition(startCellId);
    auto goal = getCellPosition(goalCellId);

    if (start == std::make_pair(-1, -1) || goal == std::make_pair(-1, -1)) {
        return {};
    }

    auto compare = [](const std::pair<int, float>& left, const std::pair<int, float>& right) {
        return left.second > right.second;
        };

    std::priority_queue<std::pair<int, float>, std::vector<std::pair<int, float>>, decltype(compare)> openSet(compare);
    std::unordered_set<int> openSetIds;
    std::unordered_set<int> closedSet;
    std::unordered_map<int, int> cameFrom;
    std::unordered_map<int, float> gScore;
    std::unordered_map<int, float> fScore;

    openSet.emplace(startCellId, 0.0f);
    openSetIds.insert(startCellId);
    gScore[startCellId] = 0.0f;
    fScore[startCellId] = Heuristic(start, goal);

    while (!openSet.empty()) {
        int current = openSet.top().first;
        openSet.pop();
        openSetIds.erase(current);

        if (current == goalCellId) {
            std::vector<int> path;
            while (current != startCellId) {
                path.push_back(current);
                current = cameFrom.at(current);
            }
            path.push_back(startCellId);
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current);
        auto currentPos = getCellPosition(current);

        std::vector<std::pair<int, glm::vec2>> neighbors = {
            { getNextCellId(current, {0.f, -1.f}), {0.f, -1.f} }, // Up
            { getNextCellId(current, {0.f, 1.f}), {0.f, 1.f} },   // Down
            { getNextCellId(current, {-1.f, 0.f}), {-1.f, 0.f} }, // Left
            { getNextCellId(current, {1.f, 0.f}), {1.f, 0.f} }    // Right
        };

        for (const auto& neighbor : neighbors) {
            int neighborId = neighbor.first;
            auto neighborPos = getCellPosition(neighborId);

            if (neighborPos == std::make_pair(-1, -1) || getCell(neighborPos.first, neighborPos.second) == CellType::Wall) {
                continue;
            }

            if (excludedNodes.find(neighborId) != excludedNodes.end() || closedSet.find(neighborId) != closedSet.end()) {
                continue;
            }

            auto currentCenter = getCellCenter(current);
            auto neighborCenter = getCellCenter(neighborId);
            float distance = std::hypot(neighborCenter.first - currentCenter.first, neighborCenter.second - currentCenter.second);

            float tentative_gScore = gScore.at(current) + distance;

            if (gScore.find(neighborId) == gScore.end() || tentative_gScore < gScore.at(neighborId)) {
                cameFrom[neighborId] = current;
                gScore[neighborId] = tentative_gScore;
                fScore[neighborId] = tentative_gScore + Heuristic(neighborPos, goal);

                if (openSetIds.find(neighborId) == openSetIds.end()) {
                    openSet.emplace(neighborId, fScore[neighborId]);
                    openSetIds.insert(neighborId);
                }
            }
        }
    }

    return {};
}

std::vector<int> Grid::FindPath(int startCellId, int goalCellId) {
    return AStarSearch(startCellId, goalCellId);
}

std::vector<int> Grid::FindSecondBestPath(int startCellId, int goalCellId) {
    std::vector<int> shortestPath = FindPath(startCellId, goalCellId);

    if (shortestPath.empty()) {
        return {};
    }

    std::unordered_set<int> excludedNodes(shortestPath.begin() + 1, shortestPath.end() - 1);

    return AStarSearch(startCellId, goalCellId, excludedNodes);
}

std::vector<int> Grid::FindFarthestPath(int startCellId, int playerCellId) {
    auto playerPos = getCellPosition(playerCellId);

    int farthestCellId = -1;
    float maxDistance = -1.0f;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (getCell(row, col) == CellType::Wall) {
                continue;
            }

            int cellId = getCellId(row, col);
            auto cellPos = std::make_pair(row, col);

            float distance = Heuristic(playerPos, cellPos);

            if (distance > maxDistance) {
                maxDistance = distance;
                farthestCellId = cellId;
            }
        }
    }

    if (farthestCellId == -1) {
        return {};
    }

    return AStarSearch(startCellId, farthestCellId);
}
