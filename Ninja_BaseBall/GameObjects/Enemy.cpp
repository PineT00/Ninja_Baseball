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
    health = maxHealth;
    Scene = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
    if (Scene != nullptr)
    {
        position = Scene->ClampByTileMap(position);
    }

    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    sf::Vector2f targetPosition = player->GetPosition();
    playerPos= targetPosition;
}

void Enemy::Update(float dt)
{
    if (isDead || player == nullptr) return;
    
    sf::Vector2f playerPosition = player->GetPosition();
    sf::Vector2f currentPosition = sprite.getPosition();
    TargetDirection(playerPosition);
    float yDistance = std::abs(playerPosition.y - currentPosition.y);
    float xDistance = std::abs(playerPosition.x - currentPosition.x);

    // Y축 위치 맞추기
    if (yDistance > acceptableYDistance) {
        currentPosition.y += (playerPosition.y > currentPosition.y ? 1 : -1) * speed * dt;
    } else {
        // Y축이 일치하면 X축 거리 유지 로직
        if (xDistance > minDistance) {
            currentPosition.x += (playerPosition.x > currentPosition.x ? 1 : -1) * speed * dt;
        }
    }

    sprite.setPosition(currentPosition);

    // 대쉬 조건: Y축이 일치하고, X축 거리가 적절한 범위 내에 있을 때
    if (yDistance <= acceptableYDistance && xDistance > minDistance && xDistance <= dashDistance && !isDash && isReadyToDash) {
        DashToPlayer();
    }

    // 공격 조건 검사
    if (player->GetHitBox().intersects(attackBox.getGlobalBounds())) {
        Attack();
    }

    UpdateAttackState(dt);
    enemyAnimator.Update(dt);
    UpdateDashState(dt);
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

    if(InputManager::GetKeyDown(sf::Keyboard::F1))
    {
        window.draw(damageBox);
        window.draw(attackBox);
    }
   
}

void Enemy::OnDamage(int damage)
{
    health -= damage;
    if(health <= 0)
    {
        isDead = true;
    }
}

void Enemy::DashTowards(const sf::Vector2f& target, float dt)
{
    if (!isReadyToDash) return;
    sf::Vector2f direction = Normalize(player->GetPosition() - sprite.getPosition());
    sprite.move(direction * speed * dt);
}

void Enemy::Attack()
{
    if(!isAttack) return;
    attackTimer = attackCooldown;
    isAttack = true;
    if(player != nullptr)
    {
        if(player->GetHitBox().intersects(attackBox.getGlobalBounds()))
        {
            //player->OnDamage(damage);
        }
    }
}

void Enemy::MoveTowards(const sf::Vector2f& target, float dt)
{
    sf::Vector2f currentPosition = sprite.getPosition();
    float distanceToTarget = Utils::MyMath::Distance(target, currentPosition);
    
    const float minDistance = 500.0f;  
    
    if (distanceToTarget > minDistance)
    {
        float yDistance = std::abs(target.y - currentPosition.y);
        float xDistance = std::abs(target.x - currentPosition.x);

        sf::Vector2f moveDirection(0.0f, 0.0f);


        if (yDistance > acceptableYDistance) {
            moveDirection.y = (target.y > currentPosition.y) ? 1.0f : -1.0f;
        }


        if (yDistance <= acceptableYDistance) {
            moveDirection.x = (target.x > currentPosition.x) ? 1.0f : -1.0f;
        }


        if (moveDirection != sf::Vector2f(0.0f, 0.0f)) {
            sf::Vector2f normalizedDirection = Normalize(moveDirection);
            sprite.move(normalizedDirection * speed * dt);
            position = sprite.getPosition(); 
        }
    }
}

sf::Vector2f Enemy::Normalize(const sf::Vector2f& source)
{
    float length = std::hypot(source.x, source.y);
    if (length != 0) {
        return sf::Vector2f(source.x / length, source.y / length);
    }
    return source;
}

void Enemy::TargetDirection(const sf::Vector2f& playerPosition)
{
    if (playerPosition.x < sprite.getPosition().x) {
        sprite.setScale(1.0f, 1.0f);
    }
    else {
        sprite.setScale(-1.0f, 1.0f);
    }
}

void Enemy::SetBox(bool flip)
{
    if (flip)
    {
        attackBox.setPosition({ sprite.getPosition().x - attackBox.getSize().x, sprite.getPosition().y - attackBox.getSize().y });
        damageBox.setPosition({ sprite.getPosition().x - damageBox.getSize().x, sprite.getPosition().y - damageBox.getSize().y });
    }
    else
    {
        attackBox.setPosition({ sprite.getPosition().x, sprite.getPosition().y - attackBox.getSize().y });
        damageBox.setPosition({ sprite.getPosition().x, sprite.getPosition().y - damageBox.getSize().y });
    }
}

void Enemy::UpdateDashState(float dt)
{
    if (!isReadyToDash) {
        dashCooldownTimer -= dt;
        if (dashCooldownTimer <= 0) {
            isReadyToDash = true;
            dashCooldownTimer = dashCooldown; // 쿨다운 시간 재설정
        }
    }
}

void Enemy::UpdateAttackState(float dt)
{
    if(isAttack)
    {
        attackTimer -= dt;
        if(attackTimer<=0)
        {
            isAttack = false;
            attackTimer = attackCooldown;
        }
    }
}

void Enemy::DashToPlayer()
{
    if (!isReadyToDash || !player || isDash) return;

    sf::Vector2f direction = Normalize(player->GetPosition() - sprite.getPosition());
    float distance = Utils::MyMath::Distance(player->GetPosition(), sprite.getPosition());

    // 대쉬 거리가 최소 거리보다 크고, 대쉬 쿨다운이 끝났을 때만 대쉬 실행
    if (distance > minDashDistance && distance <= dashDistance) {
        sprite.move(direction * dashSpeed);
        isDash = true;
    }

    // 대쉬 종료 조건 (예: 거리 체크 또는 시간)
    if (distance <= minDashDistance || dashTimer >= dashDuration) {
        isDash = false;
        isReadyToDash = false;
        dashCooldownTimer = dashCooldown; // 쿨다운 시작
    }
}

void Enemy::MoveToPlayer(float dt)
{
    if(!player) return;

    sf::Vector2f target=player->GetPosition();
    sf::Vector2f direction = Normalize(sf::Vector2f(target.x - this->GetPosition().x, target.y-this->GetPosition().y));
    sprite.move(direction * speed * dt);
    if(target.y == this->GetPosition().y)
    {
        isReadyToDash=true;
        DashToPlayer();
    }
    
}

void Enemy::MoveToPlayerDiagon(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition)
{
    sf::Vector2f direction = Normalize(targetPosition - currentPosition);
    float moveDistance = speed * dt;

    sf::Vector2f moveStep;

    // // Y축 위치를 우선적으로 맞춥니다.
    // if (std::abs(targetPosition.y - currentPosition.y) > acceptableYDistance) {
    //     moveStep.y = direction.y * moveDistance * ySpeedIncreaseFactor;
    // } else {
    //     // Y축 위치가 맞춰졌으면, X축 방향으로 이동합니다.
    //     moveStep.y = 0; // Y축 이동 중지
    // }
    //
    // // X축 이동은 Y축이 일정 범위 내로 맞춰진 후에 진행합니다.
    // if (std::abs(currentPosition.y - targetPosition.y) <= acceptableYDistance) {
    //     moveStep.x = direction.x * moveDistance;
    // } else {
    //     moveStep.x = 0; // Y축을 맞추는 중이면, X축 이동 중지
    // }

    // Y축 위치를 우선적으로 맞춥니다.
    if (std::abs(targetPosition.y - currentPosition.y) > acceptableYDistance) {
        moveStep.y = direction.y * moveDistance * ySpeedIncreaseFactor;
        moveStep.x = direction.x * moveDistance * 0.1f; // X축 이동 속도를 절반으로 줄임
    } else {
        // Y축 위치가 맞춰졌으면, X축 방향으로 이동합니다.
        moveStep.y = 0; // Y축 이동 중지
        moveStep.x = direction.x * moveDistance;
    }
    
    position += moveStep;
    sprite.setPosition(position);

    // 플레이어의 방향에 따라 몬스터의 방향을 조정합니다.
    TargetDirection(targetPosition);
}

void Enemy::MoveToPlayerX(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition)
{
    sf::Vector2f direction = Normalize(sf::Vector2f(targetPosition.x - currentPosition.x, 0.0f));
    float moveDistance = speed * dt;
    position.x += direction.x * moveDistance;
    sprite.setPosition(position);
}
