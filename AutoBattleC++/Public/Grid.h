#pragma once
#include <vector>

class GridNode;

class Grid
{
    GridNode* GridRoot = nullptr;

    GridNode* GridTail = nullptr;

    std::vector<std::vector<GridNode*>> NewGrid;

public:

    Grid();
    ~Grid();
    
    void PopulateGrid(const int InX, const int InY);

    GridNode* GetRoot() const { return GridRoot; }

    GridNode* GetTail() const { return GridTail; }

    void DrawGrid() const;

    void ClearGrid();
};
