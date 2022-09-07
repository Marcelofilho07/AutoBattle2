#include "../Public/Grid.h"
#include "../Public/GridNode.h"
#include "../Public/Character.h"
#include <iostream>


Grid::Grid() = default;

Grid::~Grid() 
{
    ClearGrid();
}

void Grid::PopulateGrid(const int InX, const int InY)
{
    NewGrid.reserve(InX);
    for(std::vector<GridNode*> Row : NewGrid)
    {
        Row.reserve(InY);
    }

    
    for (int i = 0; i < InX; i++)
    {
        for (int j = 0; j < InY; j++)
        {
            NewGrid[i][j] = &GridNode(i, j);
        }
    }

    if (NewGrid[0][0])
    {
        GridRoot = NewGrid[0][0];
    }
    if (NewGrid[InX - 1][InY - 1] != nullptr)
    {
        GridTail = NewGrid[InX - 1][InY - 1];
    }


}

void Grid::DrawGrid() const
{
    GridNode* n = GridRoot;
    GridNode* RowFirst = GridRoot;
    while (n != nullptr)
    {
        if (n->IsNodeOccupied())
        {
            if (n->GetCharInNode()->GetIsDead())
            {
                std::cout << "[" << "X" << "] ";
            }
            else
            {
                std::cout << "[" << n->GetCharInNode()->GetIcon() << "] ";
            }
        }
        else
        {
            std::cout << "[ " << "] ";
        }

        if (n->GetRightNode() != nullptr)
        {
            n = n->GetRightNode();
        }
        else if (RowFirst->GetDownNode() != nullptr)
        {
            std::cout << std::endl;
            n = RowFirst->GetDownNode();
            RowFirst = RowFirst->GetDownNode();
        }
        else
        {
            std::cout << std::endl;
            n = nullptr;
        }
    }
    std::cout << "---------------------------------------------------------------- " << std::endl;
}

void Grid::ClearGrid()
{
    for (int i = 0; i < 100; i++)
    {
        delete[] NewGrid[i];
        NewGrid[i] = nullptr;
    }

    delete[] NewGrid;
    NewGrid = nullptr;
}