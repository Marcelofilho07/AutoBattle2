#pragma once
#include <vector>


class GridNode;

class Grid
{
    std::vector<std::vector<GridNode>> VecGrid;

public:
    Grid();
    ~Grid();

    //std::vector<std::vector<GridNode>> GetVecGrid() const {return VecGrid;}
    
    void PopulateGrid(const int InX, const int InY);

    void DrawGrid() const;

    void ResizeGrid();
    void ClearGrid();
};
