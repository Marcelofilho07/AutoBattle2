#pragma once
#include <vector>

class GridNode;

class Character
{
private:
    int Index = -1;
    int Movement = 0;
    int EmpowerCharges = 0;
    int InvulnerabilityCharges = 0;
    
    float Health = 1.f;
    float MaxHealth = Health;
    float BaseDamage = 1.f;
    float DamageMultiplier = 1.f;

    bool IsInvulnerable = false;
    bool IsEmpower = false;
    
    char Icon = '0';
    char Name = '0';

    Character* Target = nullptr;


public:
    int CharacterPositionX = 0;
    int CharacterPositionY = 0;
    std::vector<std::vector<GridNode>>* Grid = nullptr;
    
    Character();

    void SetHealth(const float InHealth);

    void SetBaseDamage(const float InBaseDamage);

    void SetDamageMultiplier(const float InDamageMultiplier);

    void SetMovement(const int InMovement);

    void SetIconAndName(const char InIcon);

    void SetTarget(Character& NewTarget);

    void SetPlayerPosition(const int InX, const int InY);

    void SetIndex(const int InIndex);

    inline char GetIcon() const { return Icon; }

    inline bool GetIsDead() const { return Health < 0; }

    void ExecuteTurn();

private:
    void TakeDamage(const float InAmount);

    void Walk(int RightSteps, int UpSteps);

    void MoveTo(const int InX, const int InY);

    bool IsTargetInRange() const;

    void Attack();

    void Empower();

    void Invulnerable();
};