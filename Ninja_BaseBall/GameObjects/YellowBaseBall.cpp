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
    SetPosition({ 1000.f, 500.f });
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
    
    window.draw(damageBox);
    window.draw(attackBox);
}

void YellowBaseBall::Update(float dt)
{
    Enemy::Update(dt);
    SetBox(sprite.getScale().x < 0);
    if (attackTimer > 0) {
        attackTimer -= dt;
    }

    SetPlayerHitBox(player->GetHitBox());
    updateTimer += dt;
    if (updateTimer >= updateInterval) {
        updateTimer = 0.0f;
        playerPosition = player->GetPosition(); // 플레이어 위치 업데이트
    }
    
    TargetDirection(playerPosition);

    float distance = Utils::MyMath::Magnitude(playerPosition - sprite.getPosition());
    
    if (currentState == YellowBaseBallState::INTRO) {
        if (distance > attackDistance + prepareAttackDistance && attackTimer <= 0) {
            DashTowards(playerPosition, dt);
            yellowBaseBallAnimator.Play("animations/BaseballYellow_Move.csv");
        }
        else if (distance <= attackDistance + prepareAttackDistance && distance > attackDistance) {
            MoveTowards(playerPosition, dt);
            yellowBaseBallAnimator.Play("animations/BaseballYellow_Move.csv");
        }
        else if (distance <= attackDistance) {
            if (CheckHitBox()) { // 몬스터의 attackBox가 플레이어의 hitBox와 충돌하면 공격
                Attack();
            }
            else { // 그렇지 않으면 계속 추적
                MoveTowards(playerPosition, dt);
                yellowBaseBallAnimator.Play("animations/BaseballYellow_Move.csv");
            }
        }
    }
    SetPosition({sprite.getPosition().x, sprite.getPosition().y});
    // if (currentState == YellowBaseBallState::PREPARE_ATTACK) {
    //     prepareAttackTimer -= dt;
    //     if (prepareAttackTimer <= 0) {
    //         Attack();
    //         yellowBaseBallAnimator.Play("animations/BaseballYellow_Attack.csv");
    //     }
    // }

    attackBox.setPosition({sprite.getPosition().x,sprite.getPosition().y});
    damageBox.setPosition({sprite.getPosition().x,sprite.getPosition().y});
    sprite.setPosition(sprite.getPosition());
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
    if (playerPosition.x < position.x) {
        sprite.setScale(1.0f, 1.0f);
    }
    else {
        sprite.setScale(-1.0f, 1.0f);
    }
}

void YellowBaseBall::SetBox(bool flip)
{
    if (flip)
    {
        attackBox.setOrigin(-200, 0);
        damageBox.setOrigin(60, 25);
    }
    else
    {
        attackBox.setOrigin(220, 0);
        damageBox.setOrigin(60, 25);
    }
  
}
