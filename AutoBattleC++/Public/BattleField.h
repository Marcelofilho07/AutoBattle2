#pragma once
#include <forward_list>
#include <memory>

class Grid;
class Character;
class Types;

class BattleField
{
	Grid* GameGrid = nullptr;
	std::forward_list<Character*> CharacterList;
	int NumberOfPlayers = 0;
	bool bGameRunning = true;

public:
	BattleField();

	void Setup();

private:
	void CreateGrid(Grid& OutGrid);

	void CreateCharacter(Character& NewChar);

	void StartGame();

	void ClearGame();

	bool HandleTurn() const;
};

