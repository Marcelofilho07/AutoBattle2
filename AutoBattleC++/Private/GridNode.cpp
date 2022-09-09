#include "../Public/GridNode.h"
#include "../Public/Character.h"

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
