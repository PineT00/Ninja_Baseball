#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class Enemy:public SpriteGo
{
public:
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

    static Enemy* Create(EnemyType enemyType);
private:
    struct ClipInfo
    {
        std::string idle;
        std::string move;
        std::string attack;
        std::string dead;
        bool flipX=false;
        sf::Vector2f point;
        ClipInfo()
        = default;
        ClipInfo(std::string idle, std::string move, std::string attack, std::string dead, bool flipX, const sf::Vector2f& point)
            :idle(std::move(idle)), move(std::move(move)), attack(std::move(attack)), dead(std::move(dead)), flipX(flipX), point(point) {}
    };
protected:
    //Enemy State
    enum class EnemyState
    {
        IDLE,
        MOVE,
        ATTACK,
        DEAD
    };
    EnemyState state;

    
    
    //Enemy Attack Type
    enum class EnemyAttackType
    {
        SINGLE,
        COMBO,
        BACKJUMP
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
    void EnemyMovement(float dt);
    void EnemyPattern(float dt);
protected:
    float attackCoolTime;
    float monsterHp;
    float monsterMaxHp;
    float monsterSpeed;
    float monsterDamage;
    float monsterAttackRange;
    float monsterAttackSpeed;
};
