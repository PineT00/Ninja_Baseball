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
        enemyAnimator.Play("animations/BaseballYellow_Move.csv");
       
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
    if (sceneDev1 != nullptr)
    {
        position = sceneDev1->ClampByTileMap(position);
    }

    sf::Vector2f playerPosition = player->GetPosition();
    float xDistance = std::abs(playerPosition.x - position.x); // X축 거리
    float yDistance = std::abs(playerPosition.y - position.y); // Y축 거리

    // 플레이어와 Y축 위치가 충분히 가까운 경우에만 행동 결정
    if (yDistance <= 2.0f) // 예를 들어, Y축 위치 차이가 10 이내일 때
    {
        if (xDistance <= attackDistance)
        {
            // 공격 거리 안에 있으면 공격
            Attack();
        }
        else if (xDistance > attackDistance && xDistance <= followDistance)
        {
            // 대쉬 거리 안에 있으면 X축 방향으로 대쉬
            DashTowards(playerPosition, dt);
        }
    }
    else
    {
        // Y축 위치 차이가 큰 경우, 플레이어의 Y축 방향으로 접근
        MoveTowards(sf::Vector2f(position.x, playerPosition.y), speed, dt);
    }

    // 공격 또는 대쉬 후에는 잠시 후퇴
    RetreatAfterAction(dt);

    Enemy::Update(dt);
    sprite.setPosition(position);

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
    else
    {
        currentState = YellowBaseBallState::HURT;
    }
}

void YellowBaseBall::DashTowards(const sf::Vector2f& target, float dt)
{
    sf::Vector2f direction = Normalize(target - position);
    position += direction * (speed * 2.0f) * dt; // 대쉬 속도는 일반 속도의 2배
    currentState = YellowBaseBallState::DASH;
}

void YellowBaseBall::Attack()
{
    currentState = YellowBaseBallState::ATTACK;
}

void YellowBaseBall::RetreatAfterAction(float dt)
{
    if(currentState == YellowBaseBallState::ATTACK || currentState == YellowBaseBallState::DASH)
    {
        sf::Vector2f toPlayer = player->GetPosition() - position;
        float currentDistance=std::hypot(toPlayer.x,toPlayer.y);

        float targetRetreatDistance = 15.f;
        if(currentDistance<targetRetreatDistance)
        {
            sf::Vector2f retreatDirection = Normalize(position - player->GetPosition());

            float retreatSpeed = speed * 1.5f;
            position += retreatDirection * retreatSpeed * dt;
            sprite.setPosition(position);

            currentState = YellowBaseBallState::MOVE;
        }
    }
}

void YellowBaseBall::MoveTowards(const sf::Vector2f& target, float speed, float dt)
{
    sf::Vector2f direction = Normalize(target - position);
    float distance= Utils::MyMath::Magnitude(target - position);

    if(distance>0)
    {
        float moveDistance = speed * dt;

        sf::Vector2f moveStep=direction * std::min(moveDistance,distance);

        position += moveStep;

        currentState = YellowBaseBallState::MOVE;
    }
}

sf::Vector2f YellowBaseBall::Normalize(const sf::Vector2f& source)
{
    float length = std::sqrt(source.x * source.x + source.y * source.y);
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}
