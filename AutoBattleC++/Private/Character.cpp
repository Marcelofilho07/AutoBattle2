#include "../Public/Grid.h"
#include "../Public/GridNode.h"
#include "../Public/Character.h"

Character::Character()
{
    Health = 1.f;
    MaxHealth = Health;
    BaseDamage = 0.f;
    DamageMultiplier = 1.f;
    EmpowerCharges = 1;
    Movement = 0;
    InvulnerabilityCharges = 1;
    Target = nullptr;
    IsInvulnerable = false;
    Name = '0';
    IsEmpower = false;
    Index = 0;
    Icon = '0';
    GridPosition = nullptr;
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

	if (Health <= 0) 
	{
		Die();
	}
}


void Character::Die() 
{
    //IsDead = true;
}

void Character::Walk(int RightSteps, int UpSteps) 
{
    int Moves = Movement;
    if (RightSteps > 0)
    {
        for (int i = 0; i < RightSteps && Moves > 0; i++)
        {
            SetPlayerPosition(GridPosition->GetRightNode());
            Moves--;
        }
    }
    else
    {
        for (int i = 0; i < -RightSteps && Moves > 0; i++)
        {
            SetPlayerPosition(GridPosition->GetLeftNode());
            Moves--;
        }
    }

    if (UpSteps > 0)
    {
        for (int i = 0; i < UpSteps && Moves > 0; i++)
        {
            SetPlayerPosition(GridPosition->GetUpNode());
            Moves--;
        }
    }
    else
    {
        for (int i = 0; i < -UpSteps && Moves > 0; i++)
        {
            SetPlayerPosition(GridPosition->GetDownNode());
            Moves--;
        }
    }
}

void Character::SetPlayerPosition(GridNode* Node)
{
    if (Node != nullptr && !Node->IsNodeOccupied())
    {
        GridPosition->ClearNode();
        GridPosition = Node;
        Node->SetCharacter(this);
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
        if (CheckCloseToTarget())
        {
            Attack();
        }
        else
        {
            Walk(Target->GridPosition->GetNodePosition().x - GridPosition->GetNodePosition().x, GridPosition->GetNodePosition().y - Target->GridPosition->GetNodePosition().y);
        }

        if (CheckCloseToTarget())
        {
            Attack();
        }
    }
}

bool Character::CheckCloseToTarget()
{
    if (GridPosition->GetUpNode()->IsNodeOccupied() || GridPosition->GetDownNode()->IsNodeOccupied() || GridPosition->GetRightNode()->IsNodeOccupied() || GridPosition->GetLeftNode()->IsNodeOccupied())
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
    //Target = NewTarget;
}
