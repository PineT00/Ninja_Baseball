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

    std::function<void()> backPosition= [this] { RetreatAfterAction(); };
    yellowBaseBallAnimator.AddEvent("animations/BaseballYellow_Attack.csv", 3, backPosition);

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
    // 공격 쿨다운 감소
    if (attackTimer > 0) {
        attackTimer -= dt;
    }

    if (sceneDev1 != nullptr) {
        position = sceneDev1->ClampByTileMap(position);
    }

    sf::Vector2f playerPosition = player->GetPosition();
    float xDistance = std::abs(playerPosition.x - position.x);
    float yDistance = std::abs(playerPosition.y - position.y);

    if (currentState != YellowBaseBallState::ATTACK && currentState != YellowBaseBallState::RETREAT) {
        // 목표 위치 결정 로직 개선
        sf::Vector2f targetPosition;
        // 플레이어가 몬스터보다 앞에 있는 경우
        if (playerPosition.x > position.x) {
            targetPosition = playerPosition - sf::Vector2f(prepareAttackDistance, 0);
            if (xDistance > prepareAttackDistance + minDistance || (attackTimer <= 0 && xDistance > attackDistance)) {
                MoveTowards(targetPosition, speed, dt);
            }
        }
        // 플레이어가 몬스터보다 뒤에 있는 경우
        else {
            targetPosition = playerPosition + sf::Vector2f(prepareAttackDistance, 0);
            if (xDistance > prepareAttackDistance + minDistance || (attackTimer <= 0 && xDistance > attackDistance)) {
                MoveTowards(targetPosition, speed, dt);
            }
        }

        if (attackTimer <= 0 && xDistance <= attackDistance && yDistance <= acceptableYDistance) {
            Attack();
        }
        else {
            enemyAnimator.Play("animations/BaseballYellow_Move.csv");
        }
    }

    if (currentState == YellowBaseBallState::RETREAT) {
        RetreatAfterAction();
    }

    sprite.setPosition(position);
    Enemy::Update(dt);
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
    enemyAnimator.Play("animations/BaseballYellow_Attack.csv");
    attackTimer = attackCooldown;
    
    //currentState = YellowBaseBallState::RETREAT;
    
    
}

void YellowBaseBall::RetreatAfterAction()
{
    if (currentState == YellowBaseBallState::RETREAT)
    {
        sf::Vector2f toPlayer = position - player->GetPosition();
        float distanceToPlayer = Utils::MyMath::Magnitude(toPlayer);

        if (distanceToPlayer < retreatDistance)
        {
            sf::Vector2f retreatDirection = Normalize(toPlayer);
            position += retreatDirection * speed;
            sprite.setPosition(position);
        }
        else
        {
            // 후퇴가 충분히 이루어졌다면 다시 MOVE 상태로 전환
            currentState = YellowBaseBallState::MOVE;
            attackTimer = attackCooldown; // 공격 쿨다운 재설정
        }
    }
}

void YellowBaseBall::MoveTowards(const sf::Vector2f& target, float speed, float dt)
{
    float minDistance = 30.0f; // 몬스터와 플레이어 사이의 최소 유지 거리
    sf::Vector2f toTarget = target - position;
    float distanceToTarget = Utils::MyMath::Magnitude(toTarget);

    if (distanceToTarget > minDistance)
    {
        sf::Vector2f direction = Normalize(toTarget);
        float moveDistance = speed * dt;
        // 실제 이동 거리는 목표까지의 거리와 이동 가능 거리 중 작은 값을 선택
        sf::Vector2f moveStep = direction * std::min(moveDistance, distanceToTarget - minDistance);
        position += moveStep;
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
