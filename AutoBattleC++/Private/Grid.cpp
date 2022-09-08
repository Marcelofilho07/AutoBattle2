#include "../Public/Grid.h"
#include "../Public/GridNode.h"
#include "../Public/Character.h"
#include <iostream>


Grid::Grid()
{
    std::cout << "GRID CREATED" << std::endl;
};

Grid::~Grid()
{
    std::cout << "GRID DESTROYED" << std::endl;
};

void Grid::PopulateGrid(const int InX, const int InY)
{
    //VecGrid.resize(InY, std::vector<GridNode>(InY));
}

void Grid::DrawGrid() const
{
    int x = 0;
    // for(const std::vector<GridNode>& Row : VecGrid)
    // {
    //     int y = 0;
    //     for(const GridNode& GN : Row)
    //     {
    //         if(GN.IsNodeOccupied())
    //         {
    //             if (GN.GetCharInNode()->GetIsDead())
    //             {
    //                 std::cout << "[" << "X" << "] ";
    //             }
    //             else
    //             {
    //                 std::cout << "[" << GN.GetCharInNode()->GetIcon() << "] ";
    //             }
    //         }
    //         else
    //         {
    //             std::cout << "[ " << x << ", " << y << " ] ";
    //         }
    //         ++y;
    //     }
    //     ++x;
    //     std::cout << std::endl;
    // }
    std::cout << "---------------------------------------------------------------- " << std::endl;
}

void Grid::ResizeGrid()
{
}

void Grid::ClearGrid()
{
    // std::cout << "CAPACITY: " << VecGrid.capacity() << std::endl;
    // std::cout << "SIZE: " << VecGrid.size() << std::endl;
    // for(std::vector<GridNode> Row : VecGrid)
    // {
    //     Row.clear();
    // }
    // VecGrid.clear();
}
