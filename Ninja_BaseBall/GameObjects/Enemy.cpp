#include "pch.h"
#include "Enemy.h"

Enemy* Enemy::Create(EnemyType enemyType)
{
    Enemy* enemy = new Enemy("Enemy");
    enemy->type = enemyType;
    switch (enemyType)
    {
    case EnemyType::BASEBALL_YELLOW:
        enemy->monsterMaxHp = 100;
        enemy->monsterHp = enemy->monsterMaxHp;
        enemy->monsterSpeed = 100;
        enemy->monsterDamage = 10;
        enemy->monsterAttackRange = 50;
        enemy->monsterAttackSpeed = 1.f;
        enemy->animator.Play("animations/BaseballYellow_Idle.csv");
        enemy->attackType = EnemyAttackType::SINGLE;
        break;
    case EnemyType::BASEBALL_GREEN:
        // enemy->monsterMaxHp = 150;
        // enemy->monsterHp = enemy->monsterMaxHp;
        // enemy->monsterSpeed = 150;
        // enemy->monsterDamage = 20;
        // enemy->monsterAttackRange = 50;
        // enemy->monsterAttackSpeed = 1.f;
        // enemy->animator.Play("BaseballGreen_Idle");
        // enemy->attackType = EnemyAttackType::SINGLE;
        break;
    case EnemyType::BASEBALL_BLUE:
        // enemy->monsterMaxHp = 100;
        // enemy->monsterHp = enemy->monsterMaxHp;
        // enemy->monsterSpeed = 100;
        // enemy->monsterDamage = 10;
        // enemy->monsterAttackRange = 50;
        // enemy->monsterAttackSpeed = 1.f;
        // enemy->animator.Play("BaseballBlue_Idle");
        // enemy->attackType = EnemyAttackType::COMBO;
        break;
    case EnemyType::BASEBALL_WHITE:
        // enemy->monsterMaxHp = 100;
        // enemy->monsterHp = enemy->monsterMaxHp;
        // enemy->monsterSpeed = 100;
        // enemy->monsterDamage = 10;
        // enemy->monsterAttackRange = 50;
        // enemy->monsterAttackSpeed = 1.f;
        // enemy->animator.Play("BaseballWhite_Idle");
        // enemy->attackType = EnemyAttackType::BACKJUMP;
        break;
    case EnemyType::ELITE:
        
        break;
    case EnemyType::BOSS:
        
        break;
    default:
        break;
    }
    return enemy;
}

Enemy::Enemy(const std::string& name): SpriteGo(name), state(EnemyState::IDLE), attackCoolTime(0), monsterHp(0), monsterMaxHp(0),
                monsterSpeed(0),
                monsterDamage(0),
                monsterAttackRange(0),
                monsterAttackSpeed(0)
{
}

Enemy::~Enemy()
= default;

void Enemy::Init()
{
    SpriteGo::Init();
    animator.SetTarget(&sprite);
    SetOrigin(Origins::MC);
    //SetPosition(sf::Vector2f(100, 100));
    //SetScale(sf::Vector2f(1, 1));
    
}

void Enemy::Release()
{
    SpriteGo::Release();
}

void Enemy::Reset()
{
    SpriteGo::Reset();
    monsterHp = monsterMaxHp;
    
}

void Enemy::Update(float dt)
{
    SpriteGo::Update(dt);
    EnemyMovement(dt);
    EnemyPattern(dt);
    
}

void Enemy::LateUpdate(float dt)
{
    SpriteGo::LateUpdate(dt);
}

void Enemy::FixedUpdate(float dt)
{
    SpriteGo::FixedUpdate(dt);
}

void Enemy::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Enemy::EnemyMovement(float dt)
{
    switch (state)
    {
        case EnemyState::IDLE:
            
            break;
        case EnemyState::MOVE:
            
            break;
        case EnemyState::ATTACK:
            
            break;
        case EnemyState::DEAD:
            
            break;
    }
}

void Enemy::EnemyPattern(float dt)
{
    switch (attackType)
    {
        case EnemyAttackType::SINGLE:
            
            break;
        case EnemyAttackType::COMBO:
            
            break;
        case EnemyAttackType::BACKJUMP:
            
            break;
    }
}
