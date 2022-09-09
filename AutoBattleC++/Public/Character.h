#pragma once
#include <memory>

class BattleField;
class GridNode;

class Character
{
    float Health = 1.f;
    float MaxHealth = Health;
    float BaseDamage = 1.f;
    float DamageMultiplier = 1.f;
    
    int Movement = 0;
    int EmpowerCharges = 0;
    int InvulnerabilityCharges = 0;
    int CharacterPositionX = 0;
    int CharacterPositionY = 0;

    bool IsInvulnerable = false;
    bool IsEmpower = false;
    
    char Icon = '0';

    std::weak_ptr<Character> Target;
    BattleField* Battlefield = nullptr;
    
public:
    Character();
    Character(const float InHealth, const float InBaseDamage, const float InDamageMultiplier, const int InMovement, const int InEmpowerCharges, const int InInvulnerabilityCharges, const char InIcon, const int InX, const int InY, BattleField& BF);
    ~Character();

    inline char GetIcon() const { return Icon; }

    inline bool GetIsDead() const { return Health <= 0; }

    void ExecuteTurn();

private:
    void TakeDamage(const float InAmount);

    void SetPlayerPosition(const int InX, const int InY);

    void Walk(int UpSteps, int RightSteps);

    void MoveTo(const int InX, const int InY);

    bool IsTargetInRange() const;

    bool FindTarget();

    void Attack();

    void Empower();

    void Invulnerable();
};