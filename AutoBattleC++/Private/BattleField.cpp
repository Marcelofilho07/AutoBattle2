#include "../Public/BattleField.h"
#include "../Public/Character.h"
#include <iostream>

BattleField::BattleField() = default;

void BattleField::Setup()
{
    while(bGameRunning)
    {
        std::unique_ptr<Character> TeamAPlayer{std::make_unique<Character>()};
        std::unique_ptr<Character> TeamBPlayer{std::make_unique<Character>()};
        CreateGrid();
        std::cout << "Set First Player Stats: " << std::endl;
        CreateCharacter(*TeamAPlayer);
        std::cout << "Set Second Player Stats: " << std::endl;
        CreateCharacter(*TeamBPlayer);
        TeamAPlayer->SetPlayerPosition(0,0);
        TeamBPlayer->SetPlayerPosition(2,2);
        TeamAPlayer->SetTarget(*TeamBPlayer);
        TeamBPlayer->SetTarget(*TeamAPlayer);
        CharacterList.push_front(TeamAPlayer.get());
        CharacterList.push_front(TeamBPlayer.get());
        StartGame();
    }
}

void BattleField::CreateGrid()
{
    int X;
    int Y;

    std::cout << "Set Grid Lenght: " << std::endl;

    std::cin >> X;    
    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> X;
    }

    std::cout << "Set Grid Height: " << std::endl;

    std::cin >> Y;    
    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Y;
    }

    if ((X <= 1 && Y <= 1) || (X == 0 || Y == 0))
    {
        std::cout << "Invalid Height or Lenght. Try again" << std::endl;
        CreateGrid(); //change this
    }
    else
    {
        Grid.resize(X, std::vector<GridNode>(Y));
        int x = 0; //Check if this is really needed
        for(std::vector<GridNode>& Row : Grid)
        {
            int y = 0;
            for(GridNode& GN : Row)
            {
                GN.SetGridPosition(x,y);
                ++y;
            }
            ++x;
            std::cout << std::endl;
        }
    }
}

void BattleField::CreateCharacter(Character& NewChar)
{
    float Health;
    float Damage;
    float DamageMultiplier;
    int Movements;
    char Icon;

    std::cout << "Set new Player Health: " << std::endl;

    std::cin >> Health;
    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Health;
    }
    

    std::cout << "Set new Player Damage: " << std::endl;

    std::cin >> Damage;
    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Damage;
    }

    std::cout << "Set new Player Damage Multiplier: " << std::endl;

    std::cin >> DamageMultiplier;
    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> DamageMultiplier;
    }

    std::cout << "Set new Player number of Movements: " << std::endl;

    std::cin >> Movements;

    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Movements;
    }

    std::cout << "Set new Player Icon: " << std::endl;

    std::cin >> Icon;
    while (std::cin.fail()) 
    {
        std::cout << "Please input a single letter" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Icon;
    }
    
    NewChar.SetHealth(10);
    NewChar.SetBaseDamage(Damage);
    NewChar.SetDamageMultiplier(DamageMultiplier);
    NewChar.SetMovement(Movements);
    NewChar.SetIconAndName(Icon);
    NumberOfPlayers++;
    NewChar.SetIndex(NumberOfPlayers);
    NewChar.Grid = &Grid;
    
    std::cout << "Player Created!" << std::endl << std::endl;
}

void BattleField::StartGame()
{
    DrawGrid();
    bool IsRunning = true;

    char WaitInput;
    std::cout << "Enter any key to start the round..." << std::endl;
    std::cin >> WaitInput;

    while (IsRunning)
    {
        IsRunning = HandleTurn();
    }
    for(Character* a :  CharacterList)
    {
        if(a->GetIsDead())
        {
            std::cout << "A PLAYER IS DEAD!" << std::endl;
        }
    }

    int Choice;
    std::cout << "Do you want to start a new game? Yes - 1 / No - 0" << std::endl;
    std::cin >> Choice;

    while (std::cin.fail()) 
    {
        std::cout << "Please input a number" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> Choice;
    }

    if (!Choice)
    {
        bGameRunning = false;
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
}

bool BattleField::HandleTurn() const
{
    char WaitInput;
    for(Character* a :  CharacterList)
    {
        if(a->GetIsDead())
        {
            return false;
        }

        a->ExecuteTurn();
        DrawGrid();
        std::cout << "Enter any key to start the next turn..." << std::endl;
        std::cin >> WaitInput;
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
