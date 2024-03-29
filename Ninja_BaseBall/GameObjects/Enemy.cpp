#include "pch.h"
#include "Enemy.h"
#include "SceneDev1.h"
#include "Player.h"


void Enemy::SetState(EnemyState Enemystate,int damageCount)
{
    this->Enemystate = Enemystate;
    
    switch (Enemystate)
    {
        // case EnemyState::IDLE:
        //     
        //     break;
        case EnemyState::MOVE:
            randX = Utils::Random::RandomRange(-1, 2);
            break;
        case EnemyState::ATTACK:
            attackDirection = playerPos.x;
            attackTimer = 0.f;
            isAttackPlay = false;
            break;
        case EnemyState::DASH:
            dashDirection=playerPos.x>position.x?sf::Vector2f(1,0):sf::Vector2f(-1,0);
            dashEndPosition=playerPos;
            dashTimer = 0.f;
            break;
        case EnemyState::HURT:
            hurtTimer=0.f;
            break;
        case EnemyState::DEAD:
            deadTimer=0.f;
            break;
        case EnemyState::CATCHED:
            catchedPosition=position;
            
            break;
        default:
            break;
    }
}

void Enemy::Init()
{
    SpriteGo::Init();
    enemyAnimator.SetTarget(&sprite);
    enemyEffectAnimator.SetTarget(&enemyOnHit);
    enemyOnHit.setScale(2.f, 2.f);
    
    // enemyAnimator.AddEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv",
    //     2,[this]()
    //     {
    //            SetState(EnemyState::MOVE);
    //     });
    // enemyAnimator.AddEvent("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv",
    //     2,[this]()
    //     {
    //         SetState(EnemyState::MOVE);
    //     });
    // enemyAnimator.AddEvent("animations/Enemy/GreenBaseBall/BaseballGreen_Attack.csv",
    //     2,[this]()
    //     {
    //         SetState(EnemyState::MOVE);
    //     });
    // enemyAnimator.AddEvent("animations/Enemy/WhiteBaseBall/BaseballWhite_Attack.csv",
    //     2,[this]()
    //     {
    //         SetState(EnemyState::MOVE);
    //     });
    
}

void Enemy::UpdateIdle(float dt)
{
    
}

void Enemy::UpdateMove(float dt)
{
    sf::Vector2f currentPosition = GetPosition();
    float distanceToTarget = Utils::MyMath::Distance(playerPos, currentPosition);
    
    if (distanceToTarget < minDistance)
    {
        float yDistance = std::abs(playerPos.y - currentPosition.y);
        float xDistance = std::abs(playerPos.x - currentPosition.x);

        sf::Vector2f moveDirection(0.0f, 0.0f);
        
        if (yDistance > acceptableYDistance) {
            moveDirection.y = (playerPos.y > currentPosition.y) ? 1.0f : -1.0f;
            moveDirection.x = randX;
        }else if(xDistance > acceptableXDistance)
        {
            moveDirection.x = (playerPos.x > currentPosition.x) ? 1.0f : -1.0f;
        }

        SetFlipX(playerPos.x < currentPosition.x);
        if (moveDirection != sf::Vector2f(0.0f, 0.0f)) {
            sf::Vector2f normalizedDirection = Utils::MyMath::GetNormal(moveDirection);
            
            SetPosition(position+normalizedDirection * speed * dt);
        }
    }
    distanceToTarget = Utils::MyMath::Distance(playerPos, position);

    if(dashMaxDistance<distanceToTarget && dashTimer>=dashCooldown)
    {
        SetState(EnemyState::DASH);
    }
    else if(normalAttackDistance>distanceToTarget && attackTimer>=attackCooldown)
    {
        SetState(EnemyState::ATTACK);
    }
}

void Enemy::UpdateAttack(float dt)
{
    if(!isAttackPlay&&normalAttackDistance>Utils::MyMath::Distance(playerPos,position))
    {
        if(std::abs(playerPos.y-position.y)<20)
        {
            player->OnDamage(damage,1,position.x);
            //std::cout<<"attack"<<std::endl;
           
        }
        isAttackPlay = true;
    }
}

void Enemy::UpdateDash(float dt)
{
    SetPosition(position+dashDirection*dashSpeed*dt);
    if(Utils::MyMath::Distance(playerPos,position)<normalAttackDistance)
    {
        SetState(EnemyState::ATTACK);
    }
    else if(Utils::MyMath::Distance(dashEndPosition,position)<normalAttackDistance*0.5)
    {
        SetState(EnemyState::MOVE);
    }
}

void Enemy::UpdateHurt(float dt)
{
    hurtTimer += dt;

    if(hurtTimer>=hurtDuration)
    {
        SetState(EnemyState::MOVE);
    }
}

void Enemy::UpdateDead(float dt)
{
    deadTimer += dt;
    if(deadTimer>=deadDuration)
    {
        SetActive(false);
        isCatch=false;
    }
    
}

void Enemy::UpdateCatched(float dt)
{
    if(player->isGrip && isCatch)
    {
        SetPosition(player->GetAttackBox().getPosition());
    }
    else
    {
        isCatch = false;
        SetState(EnemyState::MOVE);
        SetPosition(catchedPosition);
    }
}

void Enemy::Update(float dt)
{
    SpriteGo::Update(dt);
    enemyAnimator.Update(dt);
    enemyEffectAnimator.Update(dt);
    enemyOnHit.setPosition(damageBox.getPosition().x, damageBox.getPosition().y - 100.f);

    attackTimer += dt;
    dashTimer += dt;
    
    if(player !=nullptr)
    {
        playerPos = player->GetPosition();
    }
    else
    {
        playerPos=GetPosition();
    }
    
    switch (Enemystate)
    {
    // case EnemyState::IDLE:
    //     UpdateIdle(dt);   
    //     break;
    case EnemyState::MOVE:
        UpdateMove(dt);    
        break;
    case EnemyState::ATTACK:
        UpdateAttack(dt);    
        break;
    case EnemyState::DASH:
        UpdateDash(dt);       
        break;
    case EnemyState::HURT:
        if(!isDead)
        {
            UpdateHurt(dt);       
        }
        break;
    case EnemyState::DEAD:
        UpdateDead(dt);      
        break;
    case EnemyState::CATCHED:
        UpdateCatched(dt);   
        break;
    default: break;
    }
}

void Enemy::Release()
{
    SpriteGo::Release();
}

void Enemy::Reset()
{
    SetSortLayer(0);
    //SpriteGo::Reset();
    scene=dynamic_cast<SceneDev1*> (SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    sf::Vector2f targetPosition = player->GetPosition();
    playerPos = targetPosition;

    attackBox.setSize({ 20,20 });
    damageBox.setSize({ 100,100 });

    attackBox.setFillColor(sf::Color::Red);
    damageBox.setFillColor(sf::Color::Blue);
    SetState(EnemyState::MOVE);
    // if (scene != nullptr)
    // {
    //     SetPosition(scene->ClampByTileMap(position));
    // }
    SetOrigin(Origins::BC);
    SetFlipX(false);
    health = maxHealth;
}

void Enemy::Draw(sf::RenderWindow& window)
{

    SpriteGo::Draw(window);

    window.draw(enemyOnHit);

    if (SCENE_MANAGER.GetDeveloperMode())
    {
        window.draw(damageBox);
        window.draw(attackBox);
    }
}

void Enemy::SetPosition(const sf::Vector2f& pos)
{
    SpriteGo::SetPosition(pos);
    damageBox.setPosition(pos);
    attackBox.setPosition(pos);
}

void Enemy::SetFlipX(bool flip)
{
    SpriteGo::SetFlipX(flip);
    
    if(flip)
    {
        attackBox.setOrigin({150.f,125.f});
        damageBox.setOrigin({50.f,150.f});
    }else
    {
        attackBox.setOrigin({-150.f,125.f});
        damageBox.setOrigin({50.f,150.f});
    }
   
}

sf::FloatRect Enemy::GetDamageBox() const
{
    return damageBox.getGlobalBounds();
}

void Enemy::OnDamage(int damage,int count)
{
    damageCount = count;
    if(!isDead)
    {
        health-=damage;
        if(health<=0)
        {
            SetState(EnemyState::DEAD);
            isDead = true;
        }
        else
        {

            SetState(EnemyState::HURT, damageCount);
        }
    }
    enemyEffectAnimator.Play("animations/Enemy/enemy_OnHit.csv");
}

void Enemy::HoldAction()
{
    SetState(EnemyState::CATCHED);
    isCatch = true;
}




   
