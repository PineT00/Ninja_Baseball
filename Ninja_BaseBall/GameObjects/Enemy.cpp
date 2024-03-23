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
    // if (isDead || player == nullptr) return;
    //
    // sf::Vector2f playerPosition = player->GetPosition();
    // sf::Vector2f currentPosition = sprite.getPosition();
    // TargetDirection(playerPosition);
    //
    // float yDistance = std::abs(playerPosition.y - currentPosition.y);
    // float xDistance = std::abs(playerPosition.x - currentPosition.x);
    //
    // // 대쉬 조건 검사
    // if (!isDash && xDistance <= 500 && xDistance > 200) {
    //     // 대쉬 시작
    //     isDash = true;
    // }
    //
    // if (isDash) {
    //     // 대쉬 실행
    //     DashToPlayer(dt);
    // } else {
    //     // 일반 이동 로직
    //     MoveToPlayerX(dt, playerPosition, currentPosition);
    // }
    //
    // enemyAnimator.Update(dt);
    // UpdateDashState(dt);

    if (isDead || player == nullptr) return;
    
    sf::Vector2f playerPosition = player->GetPosition();
    sf::Vector2f currentPosition = sprite.getPosition();
    TargetDirection(playerPosition);
    float xDistance = std::abs(playerPosition.x - currentPosition.x);

    // 플레이어와의 X축 거리가 500일 때 대쉬 조건을 충족
    if (xDistance <= 500 && !isDash) {
        isDash = true;
    }

    // 대쉬 실행
    if (isDash) {
        // 대쉬 중 플레이어와의 거리가 200 이하가 되면 대쉬를 멈춤
        if (xDistance <= 200) {
            isDash = false;
        } else {
            // 대쉬 속도로 플레이어에게 접근
            DashToPlayer(dt);
        }
    } else {
        // 플레이어에게 일반적으로 접근
        MoveTowards(playerPosition, dt);
    }

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

    if (SCENE_MANAGER.GetDeveloperMode())
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
    attackTimer = attackCooldown;
    if(player != nullptr)
    {
        if(attackBox.getGlobalBounds().intersects(player->GetHitBox()))
        {
            //player->OnDamage(damage);
            std::cout << "Player Hit" << std::endl;
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
    // 대쉬 쿨다운 관리
    if (!isReadyToDash) {
        dashCooldownTimer -= dt;
        if (dashCooldownTimer <= 0) {
            isReadyToDash = true;
            dashCooldownTimer = dashCooldown;
        }
    }
}



void Enemy::DashToPlayer(float dt)
{
    sf::Vector2f targetPosition = player->GetPosition();
    sf::Vector2f direction = Normalize(targetPosition - sprite.getPosition());
    sprite.move(direction * dashSpeed * dt);

    // 대쉬 상태 업데이트 (필요한 경우)
    UpdateDashState(dt);
}

void Enemy::MoveToPlayer(float dt)
{
    if (!isReadyToDash || !player || isDash) return;

    sf::Vector2f direction = Normalize(player->GetPosition() - sprite.getPosition());
    float distance = Utils::MyMath::Distance(player->GetPosition(), sprite.getPosition());
    
    // 목표 거리를 200으로 설정하여 대쉬
    if (distance > minDistance && distance <= dashDistance) {
        sprite.move(direction * dashSpeed);
        isDash = true;
    }
    
    if (distance <= minDistance || dashTimer >= dashDuration) {
        isDash = false;
        isReadyToDash = false;
        dashCooldownTimer = dashCooldown; // 쿨다운 시작
    }
}

void Enemy::MoveToPlayerDiagon(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition)
{
    sf::Vector2f direction = Normalize(targetPosition - currentPosition);
    float moveDistance = speed * dt;

    sf::Vector2f moveStep;
    
    // Y축 위치를 우선적으로 맞춥니다.
    if (std::abs(targetPosition.y - currentPosition.y) > acceptableYDistance) {
        moveStep.y = direction.y * moveDistance * ySpeedIncreaseFactor;
        moveStep.x = direction.x * moveDistance * 0.1f; // X축 이동 속도를 절반으로 줄임
    } else {
        moveStep.y = 0; 
        moveStep.x = direction.x * moveDistance;
    }
    
    position += moveStep;
    SetPosition(position);
    
    TargetDirection(targetPosition);
}

void Enemy::MoveToPlayerX(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition)
{
    sf::Vector2f direction = Normalize(sf::Vector2f(targetPosition.x - currentPosition.x, 0.0f));
    float moveDistance = speed * dt;
    position.x += direction.x * moveDistance;
    SetPosition(position);
}
