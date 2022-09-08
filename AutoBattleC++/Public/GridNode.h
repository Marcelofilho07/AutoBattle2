#pragma once

class Character;

struct Vector2D
{
    int x = 0;
    int y = 0;
};

class GridNode
{
    Character* CharacterInNode = nullptr;
    Vector2D GridPosition;


public:
    GridNode();

    GridNode(const int InX, const int InY);

    ~GridNode();

    Character* GetCharInNode() const { return CharacterInNode; }

    Vector2D GetNodePosition() const { return GridPosition;  }

    inline void SetGridPosition(const int InX, const int InY)
    {
        GridPosition.x = InX;
        GridPosition.y = InY;
    }

    void SetCharacterInNode(Character& InCharacter);

    void ClearNode();

    bool IsNodeOccupied() const;
};