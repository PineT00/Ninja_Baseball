#include "pch.h"
#include "YellowBaseBall.h"
#include "SceneDev1.h"
#include "Player.h"

YellowBaseBall::YellowBaseBall(const std::string& name):Enemy(name)
{
    
}

void YellowBaseBall::Init()
{
    Enemy::Init();
    yellowBaseBallAnimator.SetTarget(&sprite);

    switch (currentState)
    {
    case YellowBaseBallState::IDLE:
        enemyAnimator.Play("animations/BaseballYellow_Idle.csv");
        break;
    case YellowBaseBallState::MOVE:
        enemyAnimator.Play("animations/BaseballYellow_Idle.csv");
       
        break;
    case YellowBaseBallState::ATTACK:
            
        break;
    case YellowBaseBallState::HURT:
            
        break;
    case YellowBaseBallState::DEAD:

        break;
    }
}

void YellowBaseBall::Release()
{
    Enemy::Release();
}

void YellowBaseBall::Reset()
{
    Enemy::Reset();
    sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    health = maxHealth;
    isDead = false;
    isAttacking = false;
    currentState = YellowBaseBallState::IDLE;

    yellowBaseBallAnimator.ClearEvent();
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
    window.draw(sprite);
}

void YellowBaseBall::Update(float dt)
{
    //처음에 Idle 상태로 좌측으로 10정도 이동
    //이후에 Move 상태로 돌입하고 플레이어를 찾음
    //플레이어를 찾으면 플레이어를 향해 이동
    sf::Vector2f playerPosition = player->GetPosition();
    sf::Vector2f playerDirection=player->GetBackPosition();
    sf::Vector2f toPlayer=playerPosition-position;
    float distanceToPlayer = std::hypot(toPlayer.x, toPlayer.y);

    if(player)
    {

        bool isWatchPlayer = (playerDirection.x * toPlayer.x + playerDirection.y * toPlayer.y) < 0;
       
        if(currentState ==YellowBaseBallState::MOVE)
        {
            if(distanceToPlayer > followDistance)
            {
                sf::Vector2f direction=toPlayer / distanceToPlayer;
                position += direction * speed * dt;
            }
            else if(isWatchPlayer)
            {
                if(distanceToPlayer <= attackDistance)
                {
                    currentState = YellowBaseBallState::ATTACK;
                }
                else
                {
                    currentState = YellowBaseBallState::DASH;
                }
            }
        }
    }

    
    if(sceneDev1 !=nullptr)
    {
        position=sceneDev1->ClampByTileMap(position);
    }
    if(currentState == YellowBaseBallState::IDLE)
    {
       enemyAnimator.Play("animations/BaseballYellow_Idle.csv");
       if(!hasMovedInitial)
       {
           position.x -= initialMoveDistance*dt;
           
       }
        if(position.x <= -100.f)
        {
            hasMovedInitial = true;
            currentState = YellowBaseBallState::MOVE;
        }
    }
    else if(currentState == YellowBaseBallState::MOVE)
    {
        //enemyAnimator.Play("animations/BaseballYellow_Move.csv");
       
       
    }
    else if(currentState == YellowBaseBallState::DASH)
    {
        
    }
    else if(currentState == YellowBaseBallState::ATTACK)
    {
        
    }
    else if(currentState == YellowBaseBallState::HURT)
    {
        
    }
    else if(currentState == YellowBaseBallState::DEAD)
    {
        
    }
    Enemy::Update(dt);
    sprite.setPosition(position);
}

void YellowBaseBall::OnDamage(int damage)
{
    Enemy::OnDamage(damage);

    if(health <= 0)
    {
        currentState = YellowBaseBallState::DEAD;
    }
}
