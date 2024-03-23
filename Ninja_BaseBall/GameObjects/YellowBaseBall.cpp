﻿// YellowBaseBall.cpp
#include "pch.h"
#include "YellowBaseBall.h"
#include "SceneDev1.h"
#include "Player.h"

YellowBaseBall::YellowBaseBall(const std::string& name) : Enemy(name), player(nullptr)
{
}

void YellowBaseBall::Init()
{
    Enemy::Init();
    yellowBaseBallAnimator.SetTarget(&sprite);
    currentEnemy = EnemyState::IDLE;
    attackBox.setSize({20,20});
    damageBox.setSize({100,100});
    
    attackBox.setFillColor(sf::Color::Red);
    damageBox.setFillColor(sf::Color::Blue);

    
}

void YellowBaseBall::Release()
{
    Enemy::Release();
}

void YellowBaseBall::Reset()
{
    Enemy::Reset();
    yellowBaseBallAnimator.ClearEvent();
    sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    health = maxHealth;
    isDead = false;
    isAttack = false;
   
    //SetOrigin(Origins::BC);
    
    if (sceneDev1 != nullptr)
    {
        position = sceneDev1->ClampByTileMap(position);
    }
    
    playerBounds = player->GetGlobalBounds();
    playerPosition = player->GetPosition();
    updateTimer = 0.f;                     
    
    // damageBounds = sprite.getGlobalBounds();
    // attackBounds = sprite.getGlobalBounds();
    //
    // attackBox.setPosition({GetPosition()});
    // damageBox.setPosition({GetPosition()});
    
}

void YellowBaseBall::FixedUpdate(float dt)
{
    Enemy::FixedUpdate(dt);
}

void YellowBaseBall::LateUpdate(float dt)
{
    Enemy::LateUpdate(dt);
}

void YellowBaseBall::Draw(sf::RenderWindow& window)
{
    Enemy::Draw(window);

    if (SCENE_MANAGER.GetDeveloperMode())
    {
        window.draw(damageBox);
        window.draw(attackBox);
    }
    
}

void YellowBaseBall::Update(float dt)
{
    Enemy::Update(dt);
    
    sprite.setPosition(sprite.getPosition());
    
    attackBox.setPosition(sprite.getPosition());
    damageBox.setPosition(sprite.getPosition());

    currentEnemy = EnemyState::MOVE;

    if(attackBox.getGlobalBounds().intersects(player->GetHitBox()))
    {
        Attack();
    }
    switch (currentEnemy)
    {
    case EnemyState::IDLE:
        yellowBaseBallAnimator.Play("animations/BaseballYellow_Idle.csv");
        break;
    case EnemyState::MOVE:
        yellowBaseBallAnimator.Play("animations/BaseballYellow_Move.csv");
        break;
    case EnemyState::ATTACK:
        yellowBaseBallAnimator.PlayQueue("animations/BaseballYellow_Attack.csv");
        break;
    case EnemyState::HURT:
        yellowBaseBallAnimator.PlayQueue("animations/BaseballYellow_Hurt.csv");
        break;
    case EnemyState::DEAD:
        yellowBaseBallAnimator.PlayQueue("animations/BaseballYellow_Dead.csv");
        break;
    case EnemyState::CATCH:
        yellowBaseBallAnimator.Play("animations/BaseballYellow_Catch.csv");
        break;
    case EnemyState::DASH:
        yellowBaseBallAnimator.PlayQueue("animations/BaseballYellow_Dash.csv");
        break;
    }
}

void YellowBaseBall::OnDamage(int damage)
{
    Enemy::OnDamage(damage);

    if (health <= 0)
    {
        currentEnemy = EnemyState::DEAD;
    }
    else
    {
        currentEnemy = EnemyState::HURT;
    }
}

void YellowBaseBall::TargetDirection(const sf::Vector2f& playerPosition)
{
    if (playerPosition.x < sprite.getPosition().x) {
        sprite.setScale(1.0f, 1.0f);
        attackBox.setOrigin({200.f,150.f});
        damageBox.setOrigin({50.f,150.f});
    }
    else {
        sprite.setScale(-1.0f, 1.0f);
        attackBox.setOrigin({-200.f,150.f});
        damageBox.setOrigin({50.f,150.f});
    }
}

void YellowBaseBall::Attack()
{
    Enemy::Attack();
    currentEnemy = EnemyState::ATTACK;
}

void YellowBaseBall::DashToPlayer(float dt)
{
    Enemy::DashToPlayer(dt);
    if(isDash)
    {
        currentEnemy = EnemyState::DASH;
    }
    
}
