#include "Graph.h"
#include <unordered_set>
#include <glm/geometric.hpp>

Grid::Grid(int numRows, int numCols, float cellWidth, float cellHeight, float x, float y)
    : m_Rows(numRows), m_Cols(numCols), m_GridX(x), m_GridY(y) {
    m_RowHeights = std::vector<float>(m_Rows, cellHeight);
    m_ColWidths = std::vector<float>(m_Cols, cellWidth);
    m_Cells = std::vector<std::vector<CellType>>(m_Rows, std::vector<CellType>(m_Cols, CellType::Tunnel));
    m_CellIds.resize(m_Rows, std::vector<int>(m_Cols, 0));
    int id = 0;
    for (int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j < m_Cols; ++j) {
            m_CellIds[i][j] = id++;
        }
    }
}

void Grid::SetCell(int row, int col, CellType type) {
    if (row >= 0 && row < m_Rows && col >= 0 && col < m_Cols) {
        m_Cells[row][col] = type;
    }
}

CellType Grid::GetCell(int row, int col) const {
    if (row >= 0 && row < m_Rows && col >= 0 && col < m_Cols) {
        return m_Cells[row][col];
    }
    return CellType::Tunnel;
}

int Grid::GetCellId(int row, int col) const
{
    float cumulativeHeight = m_GridY;
    for (int i = 0; i < m_Rows; ++i) {
        if (col >= cumulativeHeight && col < cumulativeHeight + m_RowHeights[i]) {
            row = i;
            break;
        }
        cumulativeHeight += m_RowHeights[i];
    }

    float cumulativeWidth = m_GridX;
    for (int j = 0; j < m_Cols; ++j) {
        if (row >= cumulativeWidth && row < cumulativeWidth + m_ColWidths[j]) {
            col = j;
            break;
        }
        cumulativeWidth += m_ColWidths[j];
    }

    // If both row and column indices are valid, return the corresponding cell ID
    if (row != -1 && col != -1) {
        return m_CellIds[row][col];
    }
    return -1;
}

int Grid::GetCellId(const glm::vec2& position) const {
    float localX = position.x - m_GridX;
    float localY = position.y - m_GridY;

    float accumulatedWidth = 0.0f;
    int col = -1;
    for (int i = 0; i < m_Cols; ++i) {
        accumulatedWidth += m_ColWidths[i];
        if (localX < accumulatedWidth) {
            col = i;
            break;
        }
    }

    float accumulatedHeight = 0.0f;
    int row = -1;
    for (int i = 0; i < m_Rows; ++i) {
        accumulatedHeight += m_RowHeights[i];
        if (localY < accumulatedHeight) {
            row = i;
            break;
        }
    }

    if (row >= 0 && row < m_Rows && col >= 0 && col < m_Cols) {
        return GetCellId(row, col);
    }
    else {
        return -1;
    }
}

std::pair<int, int> Grid::GetCellPosition(int cellId) const {
    for (int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j < m_Cols; ++j) {
            if (m_CellIds[i][j] == cellId) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

std::pair<float, float> Grid::GetCellCenter(int cellId) const {
    auto position = GetCellPosition(cellId);
    if (position.first != -1 && position.second != -1) {
        float centerX = m_GridX;
        for (int j = 0; j < position.second; ++j) {
            centerX += m_ColWidths[j];
        }
        centerX += m_ColWidths[position.second] / 2;

        float centerY = m_GridY;
        for (int i = 0; i < position.first; ++i) {
            centerY += m_RowHeights[i];
        }
        centerY += m_RowHeights[position.first] / 2;

        return std::make_pair(centerX, centerY);
   }
    return std::make_pair(-1.0f, -1.0f);
}

void Grid::DrawGrid() const {
    float currentY = m_GridY;
    for (int i = 0; i < m_Rows; ++i) {
        float currentX = m_GridX;
        for (int j = 0; j < m_Cols; ++j) {
            float cellWidth = m_ColWidths[j];
            float cellHeight = m_RowHeights[i];

            switch (m_Cells[i][j]) {
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
        currentY += m_RowHeights[i];
    }
}

void Grid::SetRowHeight(int row, float height) {
    if (row >= 0 && row < m_Rows) {
        m_RowHeights[row] = height;
    }
}

void Grid::SetColWidth(int col, float width) {
    if (col >= 0 && col < m_Cols) {
        m_ColWidths[col] = width;
    }
}

int Grid::GetNextCellId(int currentCellId, const glm::vec2& direction) const {
    int currentRow = currentCellId / m_Cols;
    int currentCol = currentCellId % m_Cols;

    int nextRow = currentRow + static_cast<int>(direction.y);
    int nextCol = currentCol + static_cast<int>(direction.x);

    if (nextRow < 0 || nextRow >= m_Rows || nextCol < 0 || nextCol >= m_Cols) {
        return currentCellId;
    }

    return nextRow * m_Cols + nextCol;
}

int Grid::GetNextCellId5Front(int currentCellId, const glm::vec2& pacMandirection, const glm::vec2& ghostdirection) const {
    int currentRow = currentCellId / m_Cols;
    int currentCol = currentCellId % m_Cols;
    int lastValidRow = currentRow;
    int lastValidCol = currentCol;

    for (int i = 0; i < 4; ++i) {
        int nextRow = currentRow + static_cast<int>(pacMandirection.y) * (i + 1);
        int nextCol = currentCol + static_cast<int>(pacMandirection.x) * (i + 1);

        if (nextRow >= 0 && nextRow < m_Rows && nextCol >= 0 && nextCol < m_Cols) {
            glm::vec2 nextCellDirection = glm::vec2(nextCol - currentCol, nextRow - currentRow);
            if (glm::dot(nextCellDirection, ghostdirection) == -1) {
                return lastValidRow * m_Cols + lastValidCol;
            }

            if (m_Cells[nextRow][nextCol] == CellType::Wall) {
                return lastValidRow * m_Cols + lastValidCol;
            }
            lastValidRow = nextRow;
            lastValidCol = nextCol;
        }
        else {
            return lastValidRow * m_Cols + lastValidCol;
        }
    }
    return lastValidRow * m_Cols + lastValidCol;
}

float Grid::Heuristic(const std::pair<int, int>& start, const std::pair<int, int>& goal) const {
    return static_cast<float>(std::abs(start.first - goal.first) + std::abs(start.second - goal.second));
}

bool Grid::CompareNode::operator()(const std::pair<int, float>& n1, const std::pair<int, float>& n2) {
    return n1.second > n2.second;
}

std::vector<int> Grid::AStarSearch(int startCellId, int goalCellId, const std::unordered_set<int>& excludedNodes) const {
    auto start = GetCellPosition(startCellId);
    auto goal = GetCellPosition(goalCellId);

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
        auto currentPos = GetCellPosition(current);

        std::vector<std::pair<int, glm::vec2>> neighbors = {
            { GetNextCellId(current, {0.f, -1.f}), {0.f, -1.f} }, // Up
            { GetNextCellId(current, {0.f, 1.f}), {0.f, 1.f} },   // Down
            { GetNextCellId(current, {-1.f, 0.f}), {-1.f, 0.f} }, // Left
            { GetNextCellId(current, {1.f, 0.f}), {1.f, 0.f} }    // Right
        };

        for (const auto& neighbor : neighbors) {
            int neighborId = neighbor.first;
            auto neighborPos = GetCellPosition(neighborId);

            if (neighborPos == std::make_pair(-1, -1) || GetCell(neighborPos.first, neighborPos.second) == CellType::Wall) {
                continue;
            }

            if (excludedNodes.find(neighborId) != excludedNodes.end() || closedSet.find(neighborId) != closedSet.end()) {
                continue;
            }

            auto currentCenter = GetCellCenter(current);
            auto neighborCenter = GetCellCenter(neighborId);
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
    auto playerPos = GetCellPosition(playerCellId);

    int farthestCellId = -1;
    float maxDistance = -1.0f;

    for (int row = 0; row < m_Rows; ++row) {
        for (int col = 0; col < m_Cols; ++col) {
            if (GetCell(row, col) == CellType::Wall) {
                continue;
            }

            int cellId = GetCellId(row, col);
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
