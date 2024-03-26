#include "pch.h"
#include "Enemy2.h"
#include "SceneDev1.h"
#include "Player.h"


void Enemy2::SetState(EnemyState Enemystate)
{
    this->Enemystate = Enemystate;
    std::cout<<(int)Enemystate<<std::endl;
    switch (Enemystate)
    {
        // case EnemyState::IDLE:
        //     
        //     break;
        case EnemyState::MOVE:
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_MoveFront.csv");
            break;
        case EnemyState::ATTACK:
            attackTimer = 0.f;
            isAttackPlay = false;
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv");
            break;
        case EnemyState::DASH:
            dashDirection=playerPos.x>position.x?sf::Vector2f(1,0):sf::Vector2f(-1,0);
            dashEndPosition=playerPos;
            dashTimer = 0.f;
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dash.csv");
            break;
        case EnemyState::HURT:
            
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Damage1.csv");
            break;
        case EnemyState::DEAD:
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Dead.csv");
            break;
        case EnemyState::CATCHED:
            enemyAnimator.Play("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
            break;
        default:
            break;
    }
}

void Enemy2::Init()
{
    SpriteGo::Init();
    enemyAnimator.SetTarget(&sprite);

    
    enemyAnimator.AddEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv",
        2,[this]()
        {
            
            SetState(EnemyState::MOVE);
        });

    
}

void Enemy2::UpdateIdle(float dt)
{
    
}

void Enemy2::UpdateMove(float dt)
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

void Enemy2::UpdateAttack(float dt)
{
    if(!isAttackPlay&&normalAttackDistance>Utils::MyMath::Distance(playerPos,position))
    {
        player->OnDamage(damage,1,position.x);
        std::cout<<"attack"<<std::endl;
        isAttackPlay = true;
    }
}

void Enemy2::UpdateDash(float dt)
{
    SetPosition(position+dashDirection*dashSpeed*dt);
    std::cout<<Utils::MyMath::Distance(dashEndPosition,position)<<std::endl;
    std::cout<<dashSpeed * dt * 0.5<<std::endl<<std::endl;
   
    if(Utils::MyMath::Distance(playerPos,position)<normalAttackDistance)
    {
        SetState(EnemyState::ATTACK);
    }
    else if(Utils::MyMath::Distance(dashEndPosition,position)<normalAttackDistance*0.5)
    {
        SetState(EnemyState::MOVE);
    }
}

void Enemy2::UpdateHurt(float dt)
{
    
}

void Enemy2::UpdateDead(float dt)
{
    
}

void Enemy2::UpdateCatched(float dt)
{
    
}

void Enemy2::Update(float dt)
{
    SpriteGo::Update(dt);
    enemyAnimator.Update(dt);

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
        UpdateHurt(dt);       
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

void Enemy2::Release()
{
    SpriteGo::Release();
}

void Enemy2::Reset()
{
    //SpriteGo::Reset();
    Scene=dynamic_cast<SceneDev1*> (SCENE_MANAGER.GetCurrentScene());
    player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));
    sf::Vector2f targetPosition = player->GetPosition();
    playerPos = targetPosition;

    attackBox.setSize({ 20,20 });
    damageBox.setSize({ 100,100 });

    attackBox.setFillColor(sf::Color::Red);
    damageBox.setFillColor(sf::Color::Blue);
    SetState(EnemyState::MOVE);
    if (Scene != nullptr)
    {
        SetPosition(Scene->ClampByTileMap(position));
    }
    SetOrigin(Origins::BC);
    SetFlipX(false);
    
}

void Enemy2::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);

    if (SCENE_MANAGER.GetDeveloperMode())
    {
        window.draw(damageBox);
        window.draw(attackBox);
    }
}

void Enemy2::SetPosition(const sf::Vector2f& pos)
{
    SpriteGo::SetPosition(pos);
    damageBox.setPosition(pos);
    attackBox.setPosition(pos);
}

void Enemy2::SetFlipX(bool flip)
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
   
