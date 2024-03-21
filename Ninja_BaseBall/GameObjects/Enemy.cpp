#include "pch.h"
#include "Enemy.h"
#include "Player.h"
#include "SceneDev1.h"

Enemy::Enemy(const std::string& name):SpriteGo(name)
{
}

void Enemy::Init()
{
    SpriteGo::Init();
    enemyAnimator.SetTarget(&sprite);
    
}

void Enemy::Release()
{
    SpriteGo::Release();
}

void Enemy::Reset()
{
    //SpriteGo::Reset();
    health = maxHealth;
    isDead = false;
    isAttacking = false;
    //textureId = "Monster";
    enemyAnimator.ClearEvent();
}
void Enemy::Update(float dt)
{
    SpriteGo::Update(dt);
    enemyAnimator.Update(dt);
   
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
    //window.draw(sprite);
}

void Enemy::OnDamage(int damage)
{
    health -= damage;
    if(health <= 0)
    {
        isDead = true;
    }
}
