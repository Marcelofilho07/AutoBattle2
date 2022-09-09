#include "../Public/BattleField.h"
#include "../Public/Character.h"
#include <iostream>
#include <thread>
#include <chrono>

BattleField::BattleField() = default;

void BattleField::Setup()
{
    while(bIsGameRunning)
    {
        CreateGrid();
        bool bIsInvalidInput = true;
        while(bIsInvalidInput)
        {
            int NewCharInput;
            std::cout << "Number of Characters: " << NumberOfCharacters << std::endl;
            ShowMsgReceiveInput(NewCharInput, "Do you want to add a new character?(1- Yes, 0- No, 2- Create Predefined Character): ", "Please input a number.");
            if(NewCharInput == 1)
            {
                CreateCharacter();
            }
            else if(NewCharInput == 2)
            {
                CreatePreDefinedCharacter();
            }
            else
            {
                bIsInvalidInput = false;
            }
        }
        
        StartGame();
    }
}

void BattleField::AddDeadCount()
{
    ++NumberOfCharactersDead;
}

void BattleField::CreateGrid()
{
    int X;
    int Y;
    bool bInvalidInput = true;
    while(bInvalidInput)
    {
        ShowMsgReceiveInput(X, "Set Grid Height: ", "Please input a number.");
        ShowMsgReceiveInput(Y, "Set Grid Width: ", "Please input a number.");

        if ((X <= 1 && Y <= 1) || (X == 0 || Y == 0))
        {
            std::cout << "Invalid Height or Width. Please, try again." << std::endl;
        }
        else
        {
            Grid.resize(X, std::vector<GridNode>(Y));
            bInvalidInput = false;
        }
    }

    GridHeight = static_cast<int>(Grid.size());
    GridWidth = static_cast<int>(Grid[0].size());
}

void BattleField::CreateCharacter()
{
    float Health;
    float Damage;
    float DamageMultiplier;
    int Movements;
    int EmpowerCharges;
    int InvulnerabilityCharges;
    char Icon;
    int X;
    int Y;

    ShowMsgReceiveInput(Health, "Set new Character Health: ");
    ShowMsgReceiveInput(Damage, "Set new Character Damage: ");
    ShowMsgReceiveInput(DamageMultiplier, "Set new Character Damage Multiplier: ");
    ShowMsgReceiveInput(Movements, "Set new Character Movements: ");
    ShowMsgReceiveInput(EmpowerCharges, "Set new Character Empower Charges: ");
    ShowMsgReceiveInput(InvulnerabilityCharges, "Set new Character Invulnerability Charges: ");
    ShowMsgReceiveInput(Icon, "Set new Character Icon: ", "Please input a single letter."); //know issue: passing more then 1 letter causes buffer overload.

    bool bInvalidInput = true;
    while(bInvalidInput)
    {
        std::cout << "X Axis -> Vertical" << std::endl;
        std::cout << "Y Axis -> Horizontal" << std::endl;
        ShowMsgReceiveInput(X, "Set new Character X Position: ");
        ShowMsgReceiveInput(Y, "Set new Character Y Position: ");
        if((X < 0 || Y < 0) || (X >= GridHeight || Y >= GridWidth))
        {
            std::cout << "Coordinates out of range. Please, try again." << std::endl; 
        }
        else if(Grid[X][Y].IsNodeOccupied())
        {
            std::cout << "Node is occupied. Please, try again." << std::endl;
        }
        else
        {
            bInvalidInput = false;
        }
    }
    
    NumberOfCharacters++;
    std::cout << "Player Created!" << std::endl << std::endl;
    CharacterList.emplace_front(std::make_shared<Character>(Health,Damage,DamageMultiplier, Movements, EmpowerCharges, InvulnerabilityCharges, Icon, X,Y, *this));
}

void BattleField::CreatePreDefinedCharacter()
{
    char Icon;
    int X;
    int Y;

    ShowMsgReceiveInput(Icon, "Set new Character Icon: ", "Please input a single letter.");
    
    bool bInvalidInput = true;
    while(bInvalidInput)
    {
        std::cout << "X Axis -> Vertical" << std::endl;
        std::cout << "Y Axis -> Horizontal" << std::endl;
        ShowMsgReceiveInput(X, "Set new Character X Position: ");
        ShowMsgReceiveInput(Y, "Set new Character Y Position: ");
        if((X < 0 || Y < 0) || (X >= GridHeight || Y >= GridWidth))
        {
            std::cout << "Coordinates out of range. Please, try again." << std::endl; 
        }
        else if(Grid[X][Y].IsNodeOccupied())
        {
            std::cout << "Node is occupied. Please, try again." << std::endl;
        }
        else
        {
            bInvalidInput = false;
        }
    }
    
    NumberOfCharacters++;
    std::cout << "Player Created!" << std::endl << std::endl;
    CharacterList.emplace_front(std::make_shared<Character>(10.f,1.f,1.f, 1, 1, 1, Icon, X,Y, *this));
}

void BattleField::StartGame()
{
    DrawGrid();
    bool bIsRunning = true;
    std::cout << "Game Starting in..." << std::endl;
    Countdown(5);

    while (bIsRunning)
    {
        bIsRunning = HandleTurn();
    }
    for(const std::shared_ptr<Character>& a :  CharacterList)
    {
        if(!a->GetIsDead())
        {
            std::cout << "Character " << a->GetIcon() << " is Victorious!" << std::endl;
        }
    }
    int Choice;
    ShowMsgReceiveInput(Choice,"Do you want to start a new game?(Yes - 1 / No - 0): ");
    
    if (!Choice)
    {
        bIsGameRunning = false;
    }
    
    ClearGame();
}

void BattleField::ClearGame()
{
    for(std::vector<GridNode> Row : Grid)
    {
        Row.clear();
    }
    Grid.clear();
    CharacterList.clear();
    NumberOfCharacters = 0;
    NumberOfCharactersDead = 0;
}

bool BattleField::HandleTurn() const
{
    for(const std::shared_ptr<Character>& a :  CharacterList)
    {
        if(!a->GetIsDead())
        {
            a->ExecuteTurn();
            DrawGrid();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    if((NumberOfCharacters - 1)  <= NumberOfCharactersDead)
    {
        return false;
    }
    return true;
}

void BattleField::DrawGrid() const
{
    for(const std::vector<GridNode>& Row : Grid)
    {
        for(const GridNode& GN : Row)
        {
            if(GN.IsNodeOccupied())
            {
                std::cout << "[" << GN.GetCharInNode()->GetIcon() << "] ";
            }
            else
            {
                std::cout << "[ ] ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------------------------- " << std::endl;
}

void BattleField::Countdown(const int Time) const
{
    for(int i = Time; i > 0; --i)
    {
        std::cout << i << " seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

template <typename T>
void BattleField::ShowMsgReceiveInput(T& Input, const std::string& InputMsg, const std::string& FailMsg)
{
    while (std::cout << InputMsg && !(std::cin >> Input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << FailMsg << std::endl;
    }
}
