#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class Enemy:public SpriteGo
{
private:
    
protected:
    //Enemy Type
    enum class EnemyType
    {
        NONE=-1,
        BASEBALL_YELLOW,
        BASEBALL_GREEN,
        BASEBALL_BLUE,
        BASEBALL_WHITE,
        ELITE,
        BOSS
    };
    EnemyType type=EnemyType::NONE;
    
    //Enemy State
    enum class EnemyState
    {
        IDLE,
        MOVE,
        ATTACK,
        DEAD
    };
    EnemyState state;

    static Enemy* Create(EnemyType enemyType);
    
    //Enemy Attack Type
    enum class EnemyAttackType
    {
        SINGLE,
        COMBO,
        CHARGE
    };
    EnemyAttackType attackType;
    Animator animator;
    
public:
    Enemy(const std::string& name);
    
    virtual ~Enemy();
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;

protected:
    float attackCoolTime;
    float monsterHp;
    float monsterMaxHp;
    float monsterSpeed;
    float monsterDamage;
    float monsterAttackRange;
    float monsterAttackSpeed;
};
