#pragma once

class Character;

class GridNode
{
    Character* CharacterInNode = nullptr;

public:
    Character* GetCharInNode() const { return CharacterInNode; }

    void SetCharacterInNode(Character& InCharacter);

    void ClearNode();

    bool IsNodeOccupied() const;
};