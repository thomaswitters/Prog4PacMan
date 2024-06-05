#pragma once
#include <vector>
#include "Renderer.h"
#include "TransformComponent.h"

enum class CellType { Wall, Dot, Tunnel };

class Grid {
private:
    int rows;
    int cols;
    int cellSize;
    std::vector<std::vector<int>> cellIds;
    std::vector<std::vector<CellType>> cells;
public:
    Grid(int numRows, int numCols, int cellSize) : rows(numRows), cols(numCols), cellSize(cellSize){
        cells = std::vector<std::vector<CellType>>(rows, std::vector<CellType>(cols, CellType::Tunnel));
        cellIds.resize(rows, std::vector<int>(cols, 0));
        int id = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cellIds[i][j] = id++;
            }
        }
    }

    void setCell(int row, int col, CellType type) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            cells[row][col] = type;
        }
    }

    CellType getCell(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return cells[row][col];
        }
        return CellType::Tunnel;
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

    int getSize() const {
        return cellSize;
    }

    int getCellId(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return cellIds[row][col];
        }
        return -1;
    }

    int getCellId(float x, float y) const {
        int row = static_cast<int>(y / cellSize);
        int col = static_cast<int>(x / cellSize);

        if (row < 0) row = 0;
        else if (row >= rows) row = rows - 1;
        if (col < 0) col = 0;
        else if (col >= cols) col = cols - 1;

        return cellIds[row][col];
    }

    std::pair<int, int> getCellPosition(int cellId) const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (cellIds[i][j] == cellId) {
                    return std::make_pair(i, j);
                }
            }
        }
        return std::make_pair(-1, -1);
    }

    std::pair<float, float> getCellCenter(int cellId) const {
        auto position = getCellPosition(cellId);
        if (position.first != -1 && position.second != -1) {
            float centerX = (position.second + 0.5f) * cellSize;
            float centerY = (position.first + 0.5f) * cellSize;
            return std::make_pair(centerX, centerY);
        }
        return std::make_pair(-1.0f, -1.0f);
    }

    void drawGrid() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                float x = float(j) * cellSize;
                float y = float(i) * cellSize;

                switch (cells[i][j]) {
                case CellType::Tunnel:
                    dae::Renderer::GetInstance().DrawRectangle(x, y, float(cellSize), float(cellSize), { 255, 255, 255, 0 });
                    break;
                case CellType::Wall:
                    dae::Renderer::GetInstance().DrawRectangle(x, y, float(cellSize), float(cellSize), { 0, 0, 0, 0 });
                    
                    break;
                }

                dae::Renderer::GetInstance().DrawRectangle(x, y, float(cellSize), 1.0f, { 0, 0, 0, 255 });

                dae::Renderer::GetInstance().DrawRectangle(x + cellSize - 1.0f, y, 1.0f, float(cellSize), { 0, 0, 0, 255 });

                dae::Renderer::GetInstance().DrawRectangle(x, y + cellSize - 1.0f, float(cellSize), 1.0f, { 0, 0, 0, 255 });

                dae::Renderer::GetInstance().DrawRectangle(x, y, 1.0f, float(cellSize), { 0, 0, 0, 255 });
            }
        }
    }



    int getNextCellId(int currentCellId, const glm::f32vec2& direction) const {
        int currentRow = currentCellId / cols;
        int currentCol = currentCellId % rows;

        int nextRow = currentRow + static_cast<int>(direction.y);
        int nextCol = currentCol + static_cast<int>(direction.x);

        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
            return currentCellId;
        }

        return nextRow * cols + nextCol;
    }

    int getNextTunnelCellInDirection(int currentCellId, glm::f32vec2 direction) const
    {
        int nextCellId = currentCellId;

        // Keep moving in the given direction until a tunnel cell is found
        while (true) {
            nextCellId = getNextCellId(nextCellId, direction);

            std::pair<int, int> nextCellPosition = getCellPosition(nextCellId);
            if (nextCellPosition.first != -1 && nextCellPosition.second != -1) {
                CellType nextCellType = getCell(nextCellPosition.first, nextCellPosition.second);
                if (nextCellType == CellType::Tunnel) {
                    break; // Found a tunnel cell, exit loop
                }
            }
            else {
                // Hit the edge of the grid, unable to find a tunnel cell
                nextCellId = currentCellId; // Return the original cell id
                break;
            }
        }

        return nextCellId;
    }
};

