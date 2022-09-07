#pragma once

class Types;
class Grid;

class Character
{
private:
    int Index = -1;

    int Movement = 0;

    float Health = 1.f;

    float MaxHealth = Health;

    float BaseDamage = 1.f;

    float DamageMultiplier = 1.f;

    bool IsInvulnerable = false;

    bool IsEmpower = false;

    int EmpowerCharges = 0;

    int InvulnerabilityCharges = 0;

    char Icon = '0';

    char Name = '0';

    Character* Target = nullptr;

    GridNode* GridPosition = nullptr;

public:
    Character();

    void SetHealth(const float InHealth);

    void SetBaseDamage(const float InBaseDamage);

    void SetDamageMultiplier(const float InDamageMultiplier);

    void SetMovement(const int InMovement);

    void SetIconAndName(const char InIcon);

    void SetTarget(Character& NewTarget);

    void SetPlayerPosition(GridNode* Node);

    void SetIndex(const int InIndex);

    inline char GetIcon() { return Icon; }

    inline bool GetIsDead() { return Health > 0; }

    void ExecuteTurn();

private:
    void TakeDamage(const float InAmount);

    void Die();

    void Walk(int RightSteps, int UpSteps);

    bool CheckCloseToTarget();

    void Attack();

    void Empower();

    void Invulnerable();
};