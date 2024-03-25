// YellowBaseBall.cpp
#include "pch.h"
#include "YellowBaseBall.h"
#include "SceneDev1.h"
#include "Player.h"

void YellowBaseBall::SetState()
{

    switch (currentEnemy)
    {
    case EnemyState::IDLE:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Idle.csv");
        break;
    case EnemyState::MOVE:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Move.csv");
        break;
    case EnemyState::ATTACK:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv");
        break;
    case EnemyState::HURT:
        switch (damageCount)
            {
            case 1:
                yellowBaseBallAnimator.PlayQueue("animations/Enemy/YellowBaseBall/BaseballYellow_Damage1.csv");
                break;
            case 2:
                yellowBaseBallAnimator.PlayQueue("animations/Enemy/YellowBaseBall/BaseballYellow_Damage2.csv");
                break;
            case 3:
                yellowBaseBallAnimator.PlayQueue("animations/Enemy/YellowBaseBall/BaseballYellow_Damage3.csv");
                break;
            case 4:
                yellowBaseBallAnimator.PlayQueue("animations/Enemy/YellowBaseBall/BaseballYellow_Damage4.csv");
                break;
            default:
                break;
            }
        break;
    case EnemyState::DEAD:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dead.csv");
        break;
    case EnemyState::CATCHED:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
        break;
    case EnemyState::DASH:
        yellowBaseBallAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dash.csv");
        break;
    }
}

YellowBaseBall::YellowBaseBall(const std::string& name) : Enemy(name), player(nullptr)
{
}

void YellowBaseBall::Init()
{
    Enemy::Init();
    yellowBaseBallAnimator.SetTarget(&sprite);
    currentEnemy = EnemyState::MOVE;
    SetState();
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

    //SetOrigin(Origins::BC);
    
    if (sceneDev1 != nullptr)
    {
        position = sceneDev1->ClampByTileMap(position);
    }
    
    playerBounds = player->GetGlobalBounds();
    playerPosition = player->GetPosition();
    updateTimer = 0.f;                     
    
    damageBounds = sprite.getGlobalBounds();
    attackBounds = sprite.getGlobalBounds();
    
    std::function<void()> attackOn = std::bind(&Enemy::Attack, this);
    yellowBaseBallAnimator.AddEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv", 1, attackOn);
   
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

   
}

void YellowBaseBall::Update(float dt)
{
    if (!isDead)
    {
        Enemy::Update(dt);
        //sprite.setPosition(sprite.getPosition());

        attackBox.setPosition(sprite.getPosition());
        damageBox.setPosition(sprite.getPosition());
        //currentEnemy = EnemyState::MOVE;

        if (!isDead && damageBox.getGlobalBounds().intersects(player->GetGrapBox()))
        {
            isCatched = true;
            currentEnemy = EnemyState::CATCHED;
            SetState();

            SetPosition({ player->GetAttackBox().left, player->GetAttackBox().top + 70.f });
            if (player->isAttack)
            {
                if (player->gripAttackCount == 3)
                {
                    player->isGrip = false;
                    OnDamage(50, 0);
                }
                else
                {
                    OnDamage(10, 0);
                    std::cout << health << std::endl;
                }

            }


        }

        if (!isAttackCoolOn && attackBox.getGlobalBounds().intersects(player->GetHitBox()))
        {
            Attack();
            SetState();
        }

        else
        {
            if (attackCooldown >= 2.f)
            {
                currentEnemy = EnemyState::MOVE;
                SetState();
            }
            attackCooldown -= dt;
            if (attackCooldown <= 0.f)
            {
                isAttackCoolOn = false;
                attackCooldown = 2.f;
            }
        }

    }
    else
    {
        deadTimer -= dt;
    }
    if (deadTimer <= 0.f)
    {
        SetActive(false);
        damageBox.setSize({ 0,0 });
    }
    
    yellowBaseBallAnimator.Update(dt);
}

void YellowBaseBall::OnDamage(int damage,int count)
{
    if (!isInvincible)
    {
        Enemy::OnDamage(damage, count);
        //currentEnemy = EnemyState::HURT;
        SetState();
    }

}

void YellowBaseBall::TargetDirection(const sf::Vector2f& playerPosition)
{
    Enemy::TargetDirection(playerPosition);
}

void YellowBaseBall::Attack()
{
    Enemy::Attack();
    SetState();
}

void YellowBaseBall::DashToPlayer(float dt,sf::Vector2f& currentPosition)
{
    Enemy::DashToPlayer(dt,currentPosition);
    if(isDash)
    {
        currentEnemy = EnemyState::MOVE;
    }
    
}

void YellowBaseBall::NormalMovement(float dt, sf::Vector2f& currentPosition, const sf::Vector2f& playerPosition,
    float xDistance, float yDistance)
{
    Enemy::NormalMovement(dt, currentPosition, playerPosition, xDistance, yDistance);
    currentEnemy = EnemyState::MOVE;
    
}

void YellowBaseBall::StartDash(const sf::Vector2f& playerPosition, const sf::Vector2f& currentPosition)
{
    Enemy::StartDash(playerPosition, currentPosition);
    currentEnemy = EnemyState::MOVE;
   
    
}

sf::FloatRect YellowBaseBall::GetHitBox() const
{
    return Enemy::GetHitBox();

}
