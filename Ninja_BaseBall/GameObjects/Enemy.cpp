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
    //SpriteGo::Update(dt);
    if(isDead || player==nullptr) return;

    sf::Vector2f targetPosition = player->GetPosition();
    sf::Vector2f currentPosition = sprite.getPosition();
    float yDistance=std::abs(targetPosition.y-currentPosition.y);
    float xDistance=targetPosition.x -currentPosition.x;

    if(yDistance <= acceptableYDistance && std::abs(xDistance)<=acceptableYDistance)
    {
        DashToPlayer(dt);
    }
    else
    {
        MoveToPlayerDiagon(dt,targetPosition,currentPosition);
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
    float minDistance = 100.0f;
    sf::Vector2f toTarget = target - sprite.getPosition();
    float distanceToTarget = Utils::MyMath::Magnitude(toTarget);

    if (distanceToTarget > minDistance)
    {
        sf::Vector2f direction = Normalize(toTarget);
        float moveDistance = speed * dt;
        sf::Vector2f moveStep = direction * std::min(moveDistance, distanceToTarget - minDistance);
        position += moveStep;
        sprite.setPosition(position);
    }
    else
    {
        Attack();
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
    if(isReadyToDash)
    {
        dashTimer -= dt;
        if(dashTimer <= 0)
        {
            isReadyToDash = false;
            dashTimer = 1.5f;
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

void Enemy::DashToPlayer(float dt)
{
    if(!isReadyToDash || !player) return;
    
    sf::Vector2f direction = Normalize(player->GetPosition() - sprite.getPosition());
    sprite.move(direction * dashSpeed * dt);

    isReadyToDash=false;
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
        DashToPlayer(dt);
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
    } else {
        // Y축 위치가 맞춰졌으면, X축 방향으로 이동합니다.
        moveStep.y = 0; // Y축 이동 중지
    }
    
    // X축 이동은 Y축이 일정 범위 내로 맞춰진 후에 진행합니다.
    if (std::abs(currentPosition.y - targetPosition.y) <= acceptableYDistance) {
        moveStep.x = direction.x * moveDistance;
    } else {
        moveStep.x = 0; // Y축을 맞추는 중이면, X축 이동 중지
    }
    
    position += moveStep;
    sprite.setPosition(position);

    // 플레이어의 방향에 따라 몬스터의 방향을 조정합니다.
    TargetDirection(targetPosition);
}
