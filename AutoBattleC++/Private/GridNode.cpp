#include "../Public/GridNode.h"

GridNode::GridNode()
{
    SetGridPosition(0, 0);
}

GridNode::GridNode(const int InX, const int InY)
{
    SetGridPosition(InX, InY);
}

GridNode::~GridNode() {}

void GridNode::ClearNode()
{
    if (this != nullptr)
    {
        CharacterInNode = nullptr;
    }
}

bool GridNode::IsNodeOccupied()
{
    if (this == nullptr)
    {
        return false;
    }

    if (CharacterInNode != nullptr)
    {
        return true;
    }

    return false;
}