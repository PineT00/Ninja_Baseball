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
    if (Scene != nullptr)
    {
        position = Scene->ClampByTileMap(position);
    }
    if (player != nullptr)
    {
        player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    }
}

void Enemy::Update(float dt)
{
    //SpriteGo::Update(dt);
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
    
    window.draw(damageBox);
    window.draw(attackBox);
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
    sf::Vector2f direction = Normalize(target - sprite.getPosition());
    float distanceToTarget = Utils::MyMath::Magnitude(target - sprite.getPosition());

    float dashDistance = std::max(0.0f, distanceToTarget - prepareAttackDistance);
    float dashStep = std::min(dashDistance, speed * dt * 2.0f);

    position += direction * dashStep;
    
}

void Enemy::Attack()
{
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
    }
    else
    {
        Attack();
    }
}

sf::Vector2f Enemy::Normalize(const sf::Vector2f& source)
{
    float length = std::sqrt(source.x * source.x + source.y * source.y);
    if (length != 0)
        return { source.x / length, source.y / length };
    else
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
