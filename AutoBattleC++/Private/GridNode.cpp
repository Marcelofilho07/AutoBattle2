#include "../Public/GridNode.h"
#include <iostream>
#include "../Public/Character.h"

GridNode::GridNode() = default;

GridNode::GridNode(const int InX, const int InY)
{
    SetGridPosition(InX, InY);
}

GridNode::~GridNode()
{
    std::cout << "GRID NODE DESTROYED" << GridPosition.x << GridPosition.y << std::endl;
};

void GridNode::SetCharacterInNode(Character& InCharacter)
{
    CharacterInNode = &InCharacter;
}

void GridNode::ClearNode()
{
    CharacterInNode = nullptr;
}

bool GridNode::IsNodeOccupied() const
{
    if (CharacterInNode != nullptr)
    {
        return true;
    }

    return false;
}
