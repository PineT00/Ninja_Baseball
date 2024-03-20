#include "pch.h"
#include "YellowBaseBall.h"
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
        // if(enemyAnimator.GetCurrentClipId() != "animations/BaseballYellow_Move.csv")
            // {
                //     enemyAnimator.Play("animations/BaseballYellow_Move.csv");
                    // }
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
    if(player != nullptr)
    {
        sf::Vector2f playerPos = player->GetPosition();
        sf::Vector2f direction = position - playerPos;
        float distance=std::hypot(direction.x,direction.y);
        float desiredDistance=30.f;

        if(distance > desiredDistance)
        {
            direction = direction / distance;
            position.x -= direction.x * speed * dt;
            position.y -= direction.y * speed * dt;
        }
        else if(distance < desiredDistance)
        {
            float angle =std::atan2(direction.y,direction.x)+(speed*dt /desiredDistance);
            position.x=playerPos.x+std::cos(angle)*desiredDistance;
            position.y=playerPos.y+std::sin(angle)*desiredDistance;
            
        }
    }
    
    if(currentState == YellowBaseBallState::IDLE)
    {
       enemyAnimator.Play("animations/BaseballYellow_Idle.csv");
       if(!hasMovedInitial)
       {
           position.x -= initialMoveDistance*dt;
       }
        if(position.x <= -10.f)
        {
            hasMovedInitial = true;
            currentState = YellowBaseBallState::MOVE;
        }
    }
    else if(currentState == YellowBaseBallState::MOVE)
    {
        //enemyAnimator.Play("animations/BaseballYellow_Move.csv");
        
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
