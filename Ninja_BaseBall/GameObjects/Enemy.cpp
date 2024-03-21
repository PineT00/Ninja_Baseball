﻿#include "pch.h"
#include "Enemy.h"
#include "TestScene.h"
#include "TestPlayer.h"

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
    testScene = dynamic_cast<TestScene*>(SCENE_MANAGER.GetCurrentScene());
    testPlayer = dynamic_cast<TestPlayer*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("TestPlayer"));
    health = maxHealth;
    isDead = false;
    isAttacking = false;
    textureId = "Monster";
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
}

void Enemy::OnDamage(int damage)
{
    health -= damage;
    if(health <= 0)
    {
        isDead = true;
    }
}
