#include "pch.h"
#include "Enemy.h"

Enemy* Enemy::Create(EnemyType enemyType)
{
    Enemy* enemy = new Enemy("Enemy");
    enemy->type = enemyType;
    switch (enemyType)
    {
    case EnemyType::BASEBALL_YELLOW:
        enemy->textureId = "graphics/baseball_yellow.png";
        enemy->monsterMaxHp = 100;
        enemy->monsterHp = enemy->monsterMaxHp;
        enemy->monsterSpeed = 100;
        enemy->monsterDamage = 10;
        enemy->monsterAttackRange = 50;
        enemy->monsterAttackSpeed = 1.f;
        enemy->animator.Play("BaseballYellow_Idle");
        enemy->attackType = EnemyAttackType::SINGLE;
        break;
    case EnemyType::BASEBALL_GREEN:
        
        break;
    case EnemyType::BASEBALL_BLUE:
        
        break;
    case EnemyType::BASEBALL_WHITE:
        
        break;
    case EnemyType::ELITE:
        
        break;
    case EnemyType::BOSS:
        
        break;
        case 
    default: ;
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
    SetOrigin(Origins::MC);
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
