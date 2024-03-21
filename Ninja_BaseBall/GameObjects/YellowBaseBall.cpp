#include "pch.h"
#include "YellowBaseBall.h"
#include "SceneDev1.h"
#include "Player.h"

YellowBaseBall::YellowBaseBall(const std::string& name): Enemy(name), player(nullptr)
{
}

void YellowBaseBall::Init()
{
    Enemy::Init();
    yellowBaseBallAnimator.SetTarget(&sprite);
    yellowBaseBallAnimator.Play("animations/BaseballYellow_Idle.csv");
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
    isAttacking = false;

    playerBounds = player->GetGlobalBounds();
    
    damageBounds = sprite.getGlobalBounds();
    attackBounds = sprite.getGlobalBounds();
    std::function<void()> backPosition= std::bind(&YellowBaseBall::RetreatAfterAction, this);
    yellowBaseBallAnimator.AddEvent("animations/BaseballYellow_Attack.csv", 3, backPosition);
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
    //처음에 Idle 상태로 좌측으로 100정도 이동
    //이후에 Move 상태로 돌입하고 플레이어를 찾음
    //플레이어를 찾으면 플레이어를 향해 이동
    DrawBox();
   
    if(currentState!=YellowBaseBallState::ATTACK)
    {
        SetFlipX(position.x > player->GetPosition().x);
    }
    
    GetCurrentAnimationName();
    if (attackTimer > 0) {
        attackTimer -= dt;
    }
    
    if (sceneDev1 != nullptr) {
        position = sceneDev1->ClampByTileMap(position);
    }else
    {
        return;
    }
    
    sf::Vector2f playerPosition = player->GetPosition();
    TargetDirection(playerPosition);
    float distanceX = playerPosition.x - position.x;
    float distanceY = playerPosition.y - position.y;
    if(std::abs(distanceY)>acceptableYDistance)
    {
        playerPosition.y=position.y;
    }
    
    float distance = Utils::MyMath::Magnitude(playerPosition - position);

    if (currentState != YellowBaseBallState::ATTACK && currentState != YellowBaseBallState::RETREAT) {
        if (distance > attackDistance && attackTimer <= 0) {
            DashTowards(playerPosition, dt); // 공격 범위 내로 대쉬
        } else if (distance <= attackDistance) {
            Attack(); // 공격 범위 내에 있으면 공격
        } else {
            MoveTowards(playerPosition, speed, dt); // 기타 경우 플레이어 추적
        }
    }

    // 공격 전 대기 상태
    if (currentState == YellowBaseBallState::PREPARE_ATTACK) {
        prepareAttackTimer -= dt;
        if (prepareAttackTimer <= 0) {
            Attack();
        }
    }

    // 공격 시도 후 플레이어가 범위를 벗어났을 때
    if (currentState == YellowBaseBallState::ATTACK && !sprite.getGlobalBounds().intersects(playerBounds)) {
        currentState = YellowBaseBallState::MOVE; // 몬스터가 플레이어를 다시 추적하도록 상태 변경
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
    sf::Vector2f direction=Normalize(target-position);
    float distanceToTarget = Utils::MyMath::Magnitude(target - position);

    float dashDistance=std::max(0.0f,distanceToTarget-prepareAttackDistance);
    float dashStep=std::min(dashDistance,speed*dt*2.0f);

    position += direction * dashStep;

    // 대쉬 후 공격 준비 상태로 전환
    prepareAttackTimer = prepareAttackDuration; // 공격 전 대기 시간
}

void YellowBaseBall::Attack()
{
    enemyAnimator.Play("animations/BaseballYellow_Attack.csv");
    attackTimer = attackCooldown;
    //std::cout<<" player Attack"<<std::endl;
    
    //player->OnDamage(damage)
    
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
            attackTimer = attackCooldown; // 공격 쿨다운 재설정
        }
    }
}

void YellowBaseBall::MoveTowards(const sf::Vector2f& target, float speed, float dt)
{
    float minDistance = 30.0f; // 몬스터와 플레이어 사이의 최소 유지 거리
    sf::Vector2f toTarget = target - position;

    if (position.y < target.y) // 플레이어보다 위에 있을 경우
    {
        // Y축 위치 고정
        position.y = target.y;

        // X축으로만 이동
        float distanceToTargetX = std::abs(target.x - position.x);
        if (distanceToTargetX > minDistance)
        {
            sf::Vector2f directionX = (target.x < position.x) ? sf::Vector2f(-1.0f, 0.0f) : sf::Vector2f(1.0f, 0.0f);
            float moveDistance = speed * dt;
            enemyAnimator.Play("animations/BaseballYellow_Move.csv");
            sf::Vector2f moveStep = directionX * std::min(moveDistance, distanceToTargetX - minDistance);
            position.x += moveStep.x;
        }
    }
    else // 플레이어보다 아래에 있을 경우
    {
        float distanceToTarget = Utils::MyMath::Magnitude(toTarget);
        if (distanceToTarget > minDistance)
        {
            sf::Vector2f direction = Normalize(toTarget);
            float moveDistance = speed * dt;
            enemyAnimator.Play("animations/BaseballYellow_Move.csv");
            sf::Vector2f moveStep = direction * std::min(moveDistance, distanceToTarget - minDistance);
            position += moveStep;
        }
    }
}

sf::Vector2f YellowBaseBall::Normalize(const sf::Vector2f& source)
{
    float length = std::sqrt(source.x * source.x + source.y * source.y);
    if (length != 0)
        return {source.x / length, source.y / length};
    else
        return source;
}

void YellowBaseBall::GetCurrentAnimationName()
{
    std::string currentAnimationName = yellowBaseBallAnimator.GetCurrentClipId();
    //std::cout << currentAnimationName << std::endl;
}

void YellowBaseBall::SetPosition(const sf::Vector2f& pos)
{
    Enemy::SetPosition(pos);
    sprite.setPosition(pos);
}

void YellowBaseBall::DrawBox()
{
    // damageBox 설정
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    damageBox.setSize({globalBounds.width, globalBounds.height});
    damageBox.setPosition(sprite.getPosition().x + globalBounds.width / 2, sprite.getPosition().y + globalBounds.height / 2);
    damageBox.setOrigin(damageBox.getSize().x, damageBox.getSize().y);
    damageBox.setFillColor(sf::Color::Transparent); // 실제 게임에서는 투명하게 설정
    damageBox.setOutlineColor(sf::Color::Green); // 디버깅 목적
    damageBox.setOutlineThickness(1.f);

    // attackBox 설정 (예: 몬스터의 오른쪽에 위치)
    float attackBoxWidth = 20.f;
    float attackBoxHeight = 20.f;
    attackBox.setSize({attackBoxWidth, attackBoxHeight});
    attackBox.setPosition(sprite.getPosition().x + globalBounds.width / 2 + attackBoxWidth, sprite.getPosition().y + globalBounds.height / 2);
    attackBox.setOrigin(attackBox.getSize().x, attackBox.getSize().y);
    attackBox.setFillColor(sf::Color::Transparent); // 실제 게임에서는 투명하게 설정
    attackBox.setOutlineColor(sf::Color::Red); // 디버깅 목적
    attackBox.setOutlineThickness(1.f);
    
}

void YellowBaseBall::TargetDirection(const sf::Vector2f& playerPosition)
{
    if (playerPosition.x < position.x) {
        sprite.setScale(1.0f, 1.0f); // 플레이어가 왼쪽에 있음
    } else {
        sprite.setScale(-1.0f, 1.0f); // 플레이어가 오른쪽에 있음
    }
}
