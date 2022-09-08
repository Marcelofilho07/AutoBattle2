#pragma once
#include "../Public/GridNode.h"
#include <forward_list>
#include <vector>

class Character;

class BattleField
{
	std::vector<std::vector<GridNode>> Grid;
	std::forward_list<Character*> CharacterList;
	int NumberOfPlayers = 0;
	bool bGameRunning = true;

public:
	BattleField();

	void Setup();

private:
	void CreateGrid();

	void CreateCharacter(Character& NewChar);

	void StartGame();

	void ClearGame();

	bool HandleTurn() const;

	void DrawGrid() const;
};

