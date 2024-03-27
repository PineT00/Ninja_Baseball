#include "pch.h"
#include "BaseBall.h"

#include "Player.h"
#include "SceneDev1.h"

void BaseBall::SetState(EnemyState Enemystate, int damageCount)
{
    Enemy::SetState(Enemystate, damageCount);
    switch (Enemystate)
    {
    // case EnemyState::IDLE:
    //     
    //     break;
    case EnemyState::MOVE:
        enemyAnimator.Play(animationClipNames[0]);
        break;
    case EnemyState::ATTACK:
        enemyAnimator.Play(animationClipNames[1]);

        break;
    case EnemyState::DASH:
        enemyAnimator.Play(animationClipNames[2]);
        break;
    case EnemyState::HURT:
        switch (damageCount)
        {
        case 1:
            enemyAnimator.Play(animationClipNames[3]);
            break;
        case 2:
            enemyAnimator.Play(animationClipNames[4]);
            break;
        case 3:
            enemyAnimator.Play(animationClipNames[5]);
            break;
        case 4:
            enemyAnimator.Play(animationClipNames[6]);
            break;
        case 5:
            enemyAnimator.Play(animationClipNames[9]);
        default:
            enemyAnimator.Play(animationClipNames[10]);
            break;
        }
        break;
    case EnemyState::DEAD:
        enemyAnimator.Play(animationClipNames[7]);
        break;
    case EnemyState::CATCHED:
        enemyAnimator.Play(animationClipNames[8]);
        break;
    }
}

BaseBall::BaseBall(BaseBallColor color, const std::string& name): Enemy(name)
{
}

BaseBall* BaseBall::Create(BaseBallColor color, const std::string& stageName)
{
    BaseBall* baseBall = new BaseBall(color, "BaseBall" + stageName);
    baseBall->color = color;
    switch (color)
    {
    case BaseBallColor::YELLOW:
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_MoveFront.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Dash.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage1.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage2.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage3.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage4.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Dead.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/Baseball_DynamiteDamage.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
        break;
    case BaseBallColor::BLUE:
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_MoveFront.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Dash.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Damage1.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Damage2.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Damage3.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Damage4.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Dead.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Catch.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/Baseball_DynamiteDamage.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/BlueBaseBall/BaseballBlue_Catch.csv");
        break;
    case BaseBallColor::GREEN:
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_MoveFront.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Attack.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Dash.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Damage1.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Damage2.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Damage3.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Damage4.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Dead.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Catch.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/Baseball_DynamiteDamage.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/GreenBaseBall/BaseballGreen_Catch.csv");
        break;
    case BaseBallColor::WHITE:
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_MoveFront.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Attack.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Dash.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Damage1.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Damage2.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Damage3.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Damage4.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Dead.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Catch.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/YellowBaseBall/Baseball_DynamiteDamage.csv");
        baseBall->animationClipNames.emplace_back("animations/Enemy/WhiteBaseBall/BaseballWhite_Catch.csv");
        break;
    }
    baseBall->speed = (Utils::Random::RandomRange(75.f, 150.f));
    return baseBall;
}

void BaseBall::Init()
{
    Enemy::Init();
    if (color == BaseBallColor::YELLOW)
    {
        enemyAnimator.AddEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv",
                               2, [this]()
                               {
                                   SetState(EnemyState::MOVE, -1);
                               });
    }
    else if (color == BaseBallColor::BLUE)
    {
        enemyAnimator.AddEvent("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv",
                               1, [this]()
                               {
                                   //여기서 플레이어 방향쪽으로 좀 전진하고
                                   sf::Vector2f direction = Utils::MyMath::GetNormal(playerPos - this->GetPosition());
                                   float move = 10.0f;
                                   this->SetPosition(this->position + direction * move);
                               });
        enemyAnimator.AddEvent("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv",
                               4, [this]()
                               {
                                   //여기서 추가 공격
                                   if (Utils::MyMath::Distance(this->position, playerPos) <= normalAttackDistance)
                                   {
                                       player->OnDamage(damage, -1, position.x);
                                   }
                               });
        enemyAnimator.AddEvent("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv",
                               6, [this]()
                               {
                                   SetState(EnemyState::MOVE, -1);
                               });
    }
    else if (color == BaseBallColor::GREEN)
    {
        enemyAnimator.AddEvent("animations/Enemy/GreenBaseBall/BaseballGreen_Attack.csv",
                               2, [this]()
                               {
                                   SetState(EnemyState::MOVE, -1);
                               });
    }
    else if (color == BaseBallColor::WHITE)
    {
        enemyAnimator.AddEvent("animations/Enemy/WhiteBaseBall/BaseballWhite_Attack.csv",
                               2, [this]()
                               {
                                   SetState(EnemyState::MOVE, -1);
                               });
    }
    else
    {
        std::cout << "해당하는 컬러가 없습니다." << std::endl;
    }
}
