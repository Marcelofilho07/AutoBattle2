#include <iostream>
#include <ostream>
#include <cmath>
#include "../Public/BattleField.h"
#include "../Public/GridNode.h"
#include "../Public/Character.h"

Character::Character() = default;

Character::Character(const float InHealth, const float InBaseDamage, const float InDamageMultiplier,
    const int InMovement, const int InEmpowerCharges, const int InInvulnerabilityCharges, const char InIcon, const int InX, const int InY, BattleField& BF):
    Health(InHealth), MaxHealth(InHealth), BaseDamage(InBaseDamage), DamageMultiplier(InDamageMultiplier),
    Movement(InMovement), EmpowerCharges(InEmpowerCharges), InvulnerabilityCharges(InInvulnerabilityCharges), Icon(InIcon), Battlefield(&BF)
{
    std::cout << "CHAR CREATED" << std::endl;
    SetPlayerPosition(InX, InY);
}

Character::~Character() = default;

void Character::TakeDamage(const float InAmount)
{
    if (!IsInvulnerable && Health > 0)
    {
        Health -= (InAmount * DamageMultiplier);
        std::cout << Icon << " takes " << InAmount * DamageMultiplier << "!" << std::endl;
        if(Health <= 0)
        {
            std::cout << Icon << " is DEAD!" << std::endl;
            Battlefield->GetGrid()[CharacterPositionX][CharacterPositionY].ClearNode();
            Battlefield->AddDeadCount();
        }
    }
    else
    {
        std::cout << Icon << " is invulnerable and takes no damage!" << std::endl;
        IsInvulnerable = false;
        --InvulnerabilityCharges;
        std::cout << Icon << " invulnerability wears off!" << std::endl;
    }

    if ((Health * 2 <= MaxHealth) && (Health > 0))
    {
        Invulnerable();
        Empower();
    }
}

void Character::Walk(int UpSteps, int RightSteps) 
{
    int Moves = Movement;
    const int GridWidth = static_cast<int>(Battlefield->GetGrid().size()) - 1;
    const int GridHeight = static_cast<int>(Battlefield->GetGrid()[0].size()) - 1;
    if (RightSteps > 0)
    {
        for (int i = 0; i < RightSteps && Moves > 0 && CharacterPositionY < GridWidth; i++)
        {
            MoveTo(CharacterPositionX,CharacterPositionY + 1);
            Moves--;
        }
    }
    else
    {
        for (int i = 0; i < -RightSteps && Moves > 0 && CharacterPositionY > 0; i++)
        {
            MoveTo(CharacterPositionX,CharacterPositionY - 1);
            Moves--;
        }
    }

    if (UpSteps > 0)
    {
        for (int i = 0; i < UpSteps && Moves > 0 && CharacterPositionX < GridHeight; i++)
        {
            MoveTo(CharacterPositionX + 1,CharacterPositionY);
            Moves--;
        }
    }
    else
    {
        for (int i = 0; i < -UpSteps && Moves > 0 && CharacterPositionX > 0; i++)
        {
            MoveTo(CharacterPositionX - 1,CharacterPositionY);
            Moves--;
        }
    }
}

void Character::MoveTo(const int InX, const int InY)
{
    if (!Battlefield->GetGrid()[InX][InY].IsNodeOccupied())
    {
        Battlefield->GetGrid()[CharacterPositionX][CharacterPositionY].ClearNode();
        CharacterPositionX = InX;
        CharacterPositionY = InY;
        Battlefield->GetGrid()[CharacterPositionX][CharacterPositionY].SetCharacterInNode(*this);
        std::cout << Icon << " moves to " << CharacterPositionX << ", " << CharacterPositionY <<"!" << std::endl;
    }
}

void Character::SetPlayerPosition(const int InX, const int InY)
{
    if (!Battlefield->GetGrid()[InX][InY].IsNodeOccupied())
    {
        CharacterPositionX = InX;
        CharacterPositionY = InY;
        Battlefield->GetGrid()[CharacterPositionX][CharacterPositionY].SetCharacterInNode(*this);
    }
}

void Character::Empower()
{
    if (EmpowerCharges > 0)
    {
        IsEmpower = true;        
        std::cout << "Character " << Icon << " is Empowered!" << std::endl;
    }
}

void Character::Invulnerable()
{
    if (InvulnerabilityCharges > 0)
    {
        IsInvulnerable = true;
        std::cout << "Character " << Icon << " is Invulnerable!" << std::endl;
    }
}


void Character::ExecuteTurn()
{
    if (!GetIsDead())
    {
        std::cout << "Character " << Icon << " turn." << std::endl;
        if(Target.expired())
        {
            if(!FindTarget())
            {
                std::cout << "Character " << Icon << " waits." << std::endl;
                return;
            }
        }
        
        const auto Tg = Target.lock();
        if(Tg->GetIsDead())
        {
            if(!FindTarget())
            {
                std::cout << "Character " << Icon << " waits." << std::endl;
                return;
            }
        }

        if (IsTargetInRange())
        {
            Attack();
            return;
        }

        Walk(Tg->CharacterPositionX - CharacterPositionX, Tg->CharacterPositionY - CharacterPositionY);

        if (IsTargetInRange())
        {
            Attack();
        }
    }
}

bool Character::IsTargetInRange() const
{
    const auto Tg = Target.lock();
    if ((Tg->CharacterPositionX - CharacterPositionX == 1  && Tg->CharacterPositionY - CharacterPositionY == 0) ||
        (Tg->CharacterPositionX - CharacterPositionX == 0  && Tg->CharacterPositionY - CharacterPositionY == 1) ||
        (Tg->CharacterPositionX - CharacterPositionX == -1  && Tg->CharacterPositionY - CharacterPositionY == 0) ||
        (Tg->CharacterPositionX - CharacterPositionX == 0  && Tg->CharacterPositionY - CharacterPositionY == -1))
    {
        return true;
    }

    return false;
}

bool Character::FindTarget()
{
    int MinDistanceX = static_cast<int>(Battlefield->GetGrid().size());        
    int MinDistanceY = static_cast<int>(Battlefield->GetGrid()[0].size());
    for(const std::shared_ptr<Character>& Character : Battlefield->GetCharList())
    {
        if(!((Character->CharacterPositionX - CharacterPositionX) == 0 && (Character->CharacterPositionY - CharacterPositionY) == 0) &&
            std::abs(Character->CharacterPositionX - CharacterPositionX) < MinDistanceX && std::abs(Character->CharacterPositionY - CharacterPositionY) < MinDistanceY &&
            !Character->GetIsDead())
        {
            MinDistanceX = std::abs(Character->CharacterPositionX - CharacterPositionX);
            MinDistanceY = std::abs(Character->CharacterPositionY - CharacterPositionY);
            Target = Character;
        }
    }
    if(!Target.expired())
    {
        return true;
    }
    return false;
}

void Character::Attack() 
{
    const auto Tg = Target.lock();
    if (IsEmpower)
    {
        std::cout << Icon << " attacks " << Tg->GetIcon() << " with an empowered strike!" << std::endl;
        Tg->TakeDamage(BaseDamage * 2);
        IsEmpower = false;
        --EmpowerCharges;
        std::cout << "Character " << Icon << " is no longer Empowered!" << std::endl;
    }
    else
    {
        std::cout << Icon << " attacks " << Tg->GetIcon() << "!" << std::endl;
        Tg->TakeDamage(BaseDamage);
    }
}
