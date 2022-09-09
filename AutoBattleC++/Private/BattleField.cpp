#include "../Public/BattleField.h"
#include "../Public/Character.h"
#include <iostream>

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
            ShowMsgReceiveInput(NewCharInput, "Do you want to add a new character?(1- Yes, 0- No): ", "Please input a number.");
            if(NewCharInput)
            {
                CreateCharacter();
            }
            else
            {
                bIsInvalidInput = false;
            }
        }
        
        for(const std::shared_ptr<Character>& ch : CharacterList)
        {
            std::cout << ch->GetIcon() << std::endl;
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
        ShowMsgReceiveInput(X, "Set Grid Width: ", "Please input a number.");
        ShowMsgReceiveInput(Y, "Set Grid Height: ", "Please input a number.");

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

    GridWidth = static_cast<int>(Grid.size());
    GridHeight = static_cast<int>(Grid[0].size());
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
    ShowMsgReceiveInput(Icon, "Set new Character Icon: ", "Please input a single letter.");

    bool bInvalidInput = true;
    while(bInvalidInput)
    {
        ShowMsgReceiveInput(X, "Set new Character X Position: ");
        ShowMsgReceiveInput(Y, "Set new Character Y Position: ");
        if((X < 0 || Y < 0) || (X > GridWidth || Y > GridHeight))
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

void BattleField::StartGame()
{
    DrawGrid();
    bool bIsRunning = true;

    std::string WaitInput;
    std::cout << "Enter any key to start the round..." << std::endl;
    std::cin >> WaitInput;

    while (bIsRunning)
    {
        bIsRunning = HandleTurn();
    }

    DrawGrid();
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
    std::string WaitInput;
    for(const std::shared_ptr<Character>& a :  CharacterList)
    {
        a->ExecuteTurn();
        DrawGrid();
        std::cout << "Enter any key to start the next turn..." << std::endl;
        std::cin >> WaitInput;
    }
    if((NumberOfCharacters - 1)  == NumberOfCharactersDead)
    {
        return false;
    }
    return true;
}

void BattleField::DrawGrid() const
{
    int x = 0;
    for(const std::vector<GridNode>& Row : Grid)
    {
        int y = 0;
        for(const GridNode& GN : Row)
        {
            if(GN.IsNodeOccupied())
            {
                if (GN.GetCharInNode()->GetIsDead())
                {
                    std::cout << "[" << "X" << "] ";
                }
                else
                {
                    std::cout << "[" << GN.GetCharInNode()->GetIcon() << "] ";
                }
            }
            else
            {
                std::cout << "[ " << x << ", " << y << " ] ";
            }
            ++y;
        }
        ++x;
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------------------------- " << std::endl;
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