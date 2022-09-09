#pragma once
#include "../Public/GridNode.h"
#include "../Public/Character.h"
#include <forward_list>
#include <memory>
#include <string>
#include <vector>


class BattleField
{
	std::vector<std::vector<GridNode>> Grid;
	std::forward_list<std::shared_ptr<Character>> CharacterList;
	int NumberOfCharacters = 0;
	int NumberOfCharactersDead = 0;
	bool bIsGameRunning = true;
	int GridHeight = 0;
	int GridWidth = 0;

public:
	BattleField();

	void Setup();

	std::vector<std::vector<GridNode>>& GetGrid() {return Grid;}
	
	std::forward_list<std::shared_ptr<Character>>& GetCharList() {return CharacterList;}

	void AddDeadCount();

private:
	void CreateGrid();

	void CreateCharacter();

	void StartGame();

	void ClearGame();

	bool HandleTurn() const;

	void DrawGrid() const;

	template<typename T>
	void ShowMsgReceiveInput(T& Input, const std::string& InputMsg, const std::string& FailMsg = "Invalid type! Please try again!");
};



