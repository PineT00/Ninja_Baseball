// YellowBaseBall.cpp
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
    yellowBaseBallAnimator.Play("animations/BaseballYellow_Idle.csv");
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
   
    SetOrigin(Origins::BC);
    
    if (sceneDev1 != nullptr)
    {
        position = sceneDev1->ClampByTileMap(position);
    }
    
    playerBounds = player->GetGlobalBounds();
    playerPosition = player->GetPosition();
    updateTimer = 0.f;
    
    damageBounds = sprite.getGlobalBounds();
    attackBounds = sprite.getGlobalBounds();

    attackBox.setPosition({sprite.getPosition()});
    damageBox.setPosition({sprite.getPosition()});
    
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
    
    //sprite.setPosition(sprite.getPosition());

    
}

void YellowBaseBall::OnDamage(int damage)
{
    Enemy::OnDamage(damage);

    if (health <= 0)
    {
        currentState = YellowBaseBallState::DEAD;
    }
    else
    {
        currentState = YellowBaseBallState::HURT;
    }
}

void YellowBaseBall::TargetDirection(const sf::Vector2f& playerPosition)
{
    if (playerPosition.x < sprite.getPosition().x) {
        sprite.setScale(1.0f, 1.0f);
    }
    else {
        sprite.setScale(-1.0f, 1.0f);
    }
}

void YellowBaseBall::Attack()
{
    Enemy::Attack();
}
