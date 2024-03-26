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
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_MoveFront.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Attack.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Dash.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage1.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage2.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage3.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Damage4.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Dead.csv");
            baseBall->animationClipNames.push_back("animations/Enemy/YellowBaseBall/BaseballYellow_Catch.csv");
            break;
        case BaseBallColor::BLUE:

            
            break;
        case BaseBallColor::GREEN:

            
            break;
        case BaseBallColor::WHITE:
            break;
        default:
            break;

        
    }
    return baseBall;
}
