#include "../Public/GridNode.h"
#include "../Public/Character.h"

Character::Character() = default;

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

void Character::SetIconAndName(const char InIcon)
{
    Icon = InIcon;
    Name = InIcon;
}

void Character::TakeDamage(const float InAmount) 
{
    if (!IsInvulnerable)
    {
        Health -= (InAmount * DamageMultiplier);
    }
    else
    {
        IsInvulnerable = false;
        InvulnerabilityCharges--;
    }

    if (Health * 2 <= MaxHealth)
    {
        Invulnerable();
        Empower();
    }
}

void Character::Walk(int RightSteps, int UpSteps) 
{
    int Moves = Movement;
    const int GridWidth = static_cast<int>((*Grid).size()) - 1;
    const int GridHeight = static_cast<int>((*Grid)[0].size()) - 1;
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
    if (!(*Grid)[InX][InY].IsNodeOccupied())
    {
        (*Grid)[CharacterPositionX][CharacterPositionY].ClearNode();
        CharacterPositionX = InX;
        CharacterPositionY = InY;
        (*Grid)[CharacterPositionX][CharacterPositionY].SetCharacterInNode(*this);
    }
}

void Character::SetPlayerPosition(const int InX, const int InY)
{
    if (!(*Grid)[InX][InY].IsNodeOccupied())
    {
        CharacterPositionX = InX;
        CharacterPositionY = InY;
        (*Grid)[CharacterPositionX][CharacterPositionY].SetCharacterInNode(*this);
    }
}

void Character::SetIndex(const int InIndex)
{
    Index = InIndex;
}

void Character::Empower()
{
    if (EmpowerCharges)
    {
        IsEmpower = true;
    }
}

void Character::Invulnerable()
{
    if (InvulnerabilityCharges)
    {
        IsInvulnerable = true;
    }
}


void Character::ExecuteTurn()
{
    if (!GetIsDead())
    {
        if (IsTargetInRange())
        {
            Attack();
        }
        else
        {
            Walk(Target->CharacterPositionY - CharacterPositionY, Target->CharacterPositionX - CharacterPositionX);
        }

        if (IsTargetInRange())
        {
            Attack();
        }
    }
}

bool Character::IsTargetInRange() const
{
    if ((Target->CharacterPositionX - CharacterPositionX == 1  && Target->CharacterPositionY - CharacterPositionY == 0) ||
        (Target->CharacterPositionX - CharacterPositionX == 0  && Target->CharacterPositionY - CharacterPositionY == 1) ||
        (Target->CharacterPositionX - CharacterPositionX == -1  && Target->CharacterPositionY - CharacterPositionY == 0) ||
        (Target->CharacterPositionX - CharacterPositionX == 0  && Target->CharacterPositionY - CharacterPositionY == -1))
    {
        return true;
    }

    return false;
}

void Character::Attack() 
{
    if (IsEmpower)
    {
        Target->TakeDamage(BaseDamage * 2);
        IsEmpower = false;
        EmpowerCharges--;
    }
    else
    {
        Target->TakeDamage(BaseDamage);
    }
}

void Character::SetTarget(Character& NewTarget)
{
    Target = &NewTarget;
}
