#include "../Public/GridNode.h"
#include "../Public/Character.h"

#include <iostream>
#include <ostream>
#include <cmath>
#include "../Public/BattleField.h"

Character::Character() = default;

Character::Character(const float& InHealth, const float& InBaseDamage, const float& InDamageMultiplier,
    const int& InMovement, const int& InEmpowerCharges, const int& InInvulnerabilityCharges, const char& InIcon, const int& InX, const int& InY, BattleField& BF):
    Health(InHealth), MaxHealth(InHealth), BaseDamage(InBaseDamage), DamageMultiplier(InDamageMultiplier),
    Movement(InMovement), EmpowerCharges(InEmpowerCharges), InvulnerabilityCharges(InInvulnerabilityCharges), Icon(InIcon), Battlefield(&BF)
{
    std::cout << "CHAR CREATED" << std::endl;
    SetPlayerPosition(InX, InY);
}

Character::~Character()
{
    std::cout << "CHAR DESTROYED" << std::endl;
}

void Character::SetHealth(const float InHealth)
{
    MaxHealth = InHealth;
    Health = InHealth;
}

void Character::SetBaseDamage(const float InBaseDamage)
{
    BaseDamage = InBaseDamage;
}

void Character::SetDamageMultiplier(const float InDamageMultiplier)
{
    DamageMultiplier = InDamageMultiplier;
}

void Character::SetMovement(const int InMovement)
{
    Movement = InMovement;
}

void Character::SetIcon(const char InIcon)
{
    Icon = InIcon;
}

void Character::TakeDamage(const float InAmount)
{
    if (!IsInvulnerable && Health > 0)
    {
        Health -= (InAmount * DamageMultiplier);
        std::cout << Icon << " takes " << InAmount * DamageMultiplier << "!" << std::endl;
        if(Health <= 0)
        {
            std::cout << Icon << " is DEAD!" << std::endl;
            Battlefield->AddDeadCount();
        }
    }
    else
    {
        IsInvulnerable = false;
        InvulnerabilityCharges--;
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
    }
}

void Character::SetPlayerPosition(const int InX, const int InY)
{
    if (!Battlefield->GetGrid()[InX][InX].IsNodeOccupied())
    {
        CharacterPositionX = InX;
        CharacterPositionY = InY;
        Battlefield->GetGrid()[CharacterPositionX][CharacterPositionY].SetCharacterInNode(*this);
    }
}

void Character::SetBattlefield(BattleField& BF)
{
    Battlefield = &BF;
}

void Character::Empower()
{
    if (EmpowerCharges)
    {
        IsEmpower = true;        
        std::cout << "Character " << Icon << " is Empowered!" << std::endl;
    }
}

void Character::Invulnerable()
{
    if (InvulnerabilityCharges)
    {
        IsInvulnerable = true;
        std::cout << "Character " << Icon << " is Invulnerable!" << std::endl;
    }
}


void Character::ExecuteTurn()
{
    if (!GetIsDead())
    {
        if(Target.expired())
        {
            if(!FindTarget())
            {
                return;
            }
        }
        
        const auto Tg = Target.lock();
        if(Tg->GetIsDead())
        {
            if(!FindTarget())
            {
                return;
            }
        }

        bool CanAttack = true;
        if (IsTargetInRange())
        {
            Attack();
            CanAttack = false;
        }
        else
        {
            Walk(Tg->CharacterPositionX - CharacterPositionX, Tg->CharacterPositionY - CharacterPositionY);
        }

        if (IsTargetInRange() && CanAttack)
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
        if((Character->CharacterPositionX - CharacterPositionX) != 0 && (Character->CharacterPositionY - CharacterPositionY) != 0 &&
            std::abs(Character->CharacterPositionX - CharacterPositionX) < MinDistanceX && std::abs(Character->CharacterPositionY - CharacterPositionY) < MinDistanceY &&
            !Character->GetIsDead())
        {
            MinDistanceX = (Character->CharacterPositionX - CharacterPositionX);
            MinDistanceY = (Character->CharacterPositionY - CharacterPositionY);
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
    std::cout << Icon << " attacks " << Tg->GetIcon() << "!" << std::endl;
    if (IsEmpower)
    {
        Tg->TakeDamage(BaseDamage * 2);
        IsEmpower = false;
        EmpowerCharges--;
    }
    else
    {
        Tg->TakeDamage(BaseDamage);
    }
}
