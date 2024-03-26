#include "pch.h"
#include "BaseBall.h"

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
            default:
            std::cout<< "Error" << std::endl;
                break;
            }
        break;
        case EnemyState::DEAD:
            enemyAnimator.Play(animationClipNames[7]);
            break;
        case EnemyState::CATCHED:
            enemyAnimator.Play(animationClipNames[8]);
            break;
        default:
            break;
    }
}

BaseBall::BaseBall(BaseBallColor color): Enemy("BaseBall")
{
    
}

BaseBall* BaseBall::Create(BaseBallColor color)
{
    BaseBall* baseBall = new BaseBall(color);
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
            break;
        default:
            break;

        
    }
    return baseBall;
}

void BaseBall::Init()
{
    Enemy::Init();
    if(color==BaseBallColor::YELLOW)
    {
        enemyAnimator.AddEvent("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv",
        2,[this]()
        {
            SetState(EnemyState::MOVE,-1);
        });
    }
    else if(color==BaseBallColor::BLUE)
    {
        enemyAnimator.AddEvent("animations/Enemy/BlueBaseBall/BaseballBlue_Attack.csv",
        2,[this]()
        {
            SetState(EnemyState::MOVE,-1);
        });
    }
    else if (color== BaseBallColor::GREEN)
    {
        enemyAnimator.AddEvent("animations/Enemy/GreenBaseBall/BaseballGreen_Attack.csv",
        2,[this]()
        {
            SetState(EnemyState::MOVE,-1);
        });
    }
    else if(color==BaseBallColor::WHITE)
    {
        enemyAnimator.AddEvent("animations/Enemy/WhiteBaseBall/BaseballWhite_Attack.csv",
        2,[this]()
        {
            SetState(EnemyState::MOVE,-1);
        });
    }
    else
    {
        std::cout<<"해당하는 컬러가 없습니다."<<std::endl;
    }
    
}
