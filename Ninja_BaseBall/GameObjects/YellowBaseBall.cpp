// YellowBaseBall.cpp
#include "pch.h"
#include "YellowBaseBall.h"
#include "SceneDev1.h"
#include "Player.h"

void YellowBaseBall::SetState(int damageCount)
{

    switch (currentEnemy)
    {
    case EnemyState::IDLE:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Idle.csv");
        break;
    case EnemyState::MOVE:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Move.csv");
        break;
    case EnemyState::ATTACK:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv");
        break;
    case EnemyState::HURT:
        switch (damageCount)
            {
            case 1:
                enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Damage1.csv");
                break;
            case 2:
                enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Damage2.csv");
                break;
            case 3:
                enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Damage3.csv");
                break;
            case 4:
                enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Damage4.csv");
                break;
            default:
                std::cout<< "Error" << std::endl;
                break;

            }
        
        break;
    case EnemyState::DEAD:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dead.csv");
        break;
    case EnemyState::CATCHED:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
        break;
    case EnemyState::DASH:
        enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dash.csv");
        break;
    }
}

YellowBaseBall::YellowBaseBall(const std::string& name) : Enemy(name), player(nullptr)
{
}

void YellowBaseBall::Init()
{
    Enemy::Init();
    enemyAnimator.SetTarget(&sprite);
    currentEnemy = EnemyState::MOVE;
    SetState();
    health = maxHealth;
}

void YellowBaseBall::Release()
{
    Enemy::Release();
}

void YellowBaseBall::Reset()
{
    Enemy::Reset();
    enemyAnimator.ClearEvent();
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
    
    // std::function<void()> attackOn = std::bind(&Enemy::Damage,this);
    // yellowBaseBallAnimator.SetClipEndEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv", attackOn);
    enemyAnimator.SetClipEndEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv", [this]() {
        std::cout << "Attack animation ended, calling Damage." << std::endl;
        this->Damage(); // Attack 애니메이션이 끝난 후 Damage 메서드 호출
    });
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
        
        if(attackBox.getGlobalBounds().intersects(player->GetAttackBox()))
        {
            if (isAttackReady)
            {
                isAttackReady = false;
                isAttack = true;
                isAttackCoolOn = true;
                currentEnemy = EnemyState::ATTACK;
                SetState();
            }
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

    if(isDead)
    {
        deadTimer -= dt;
        if(deadTimer<=0)
        {
            SetActive(false);
            
        }
    }
    
    enemyAnimator.Update(dt);
}

void YellowBaseBall::OnDamage(int damage,int count)
{
    //Enemy::OnDamage(damage,count);
    health -= damage;
    
    if(health>0)
    {
        currentEnemy = EnemyState::HURT;
        damageCount=count;
        SetState(count);
    }
    else
    {
        currentEnemy = EnemyState::DEAD;
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
    if(isDash && player->isGrip==false)
    {
        currentEnemy = EnemyState::DASH;
        SetState();
    }
    
}

void YellowBaseBall::NormalMovement(float dt, sf::Vector2f& currentPosition, const sf::Vector2f& playerPosition,
    float xDistance, float yDistance)
{
    if(player->isGrip==false)
    {
        Enemy::NormalMovement(dt, currentPosition, playerPosition, xDistance, yDistance);
        currentEnemy = EnemyState::MOVE;
        SetState();
    }
}

void YellowBaseBall::StartDash(const sf::Vector2f& playerPosition, const sf::Vector2f& currentPosition)
{
    if(player->isGrip==false)
    {
        Enemy::StartDash(playerPosition, currentPosition);
        currentEnemy = EnemyState::MOVE;
        SetState();                    
    
    }
}

sf::FloatRect YellowBaseBall::GetHitBox() const
{
    return Enemy::GetHitBox();

}

void YellowBaseBall::Damage()
{
    Enemy::Damage();
    currentEnemy = EnemyState::MOVE;
    SetState();
}

