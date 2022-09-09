#pragma once

class Character;

class GridNode
{
    Character* CharacterInNode = nullptr;

public:
    GridNode();

    GridNode(const int InX, const int InY);

    ~GridNode();

    Character* GetCharInNode() const { return CharacterInNode; }

    void SetCharacterInNode(Character& InCharacter);

    void ClearNode();

    bool IsNodeOccupied() const;
};