#include "pch.h"
#include "Bat.h"

Bat::Bat(BatColor color, const std::string& name): Enemy(name)
{
}

Bat* Bat::Create(BatColor color, const std::string& stageName)
{
    Bat* bat = new Bat(color, "BaseBall" + stageName);
    bat->color = color;
    switch (color)
    {
    case BatColor::YELLOW:
        //move
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_MoveFront1.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_MoveFront2.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_MoveFront3.csv");
    //attack
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Attack1.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Attack2.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Attack3.csv");
    //dash
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Dash1.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Dash2.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Dash3.csv");
    //hurt
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Damage1.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Damage2.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Damage3.csv");
    //dead
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Dead.csv");
    //catched
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Catch1.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Catch2.csv");
        bat->animationClipNames.emplace_back("animations/Enemy/Bat/BaseballBat_Catch3.csv");
        break;
    }
    bat ->speed = (Utils::Random::RandomRange(75.f,150.f));
    bat ->dashSpeed = 150.f;
    return bat;
}

void Bat::Init()
{
    Enemy::Init();


    enemyAnimator.AddEvent("animations/Enemy/Bat/BaseballBat_Attack1.csv",
                           14, [this]()
                           {
                               SetState(EnemyState::MOVE, -1);
                           });


    enemyAnimator.AddEvent("animations/Enemy/Bat/BaseballBat_Attack2.csv",
                           14, [this]()
                           {
                               SetState(EnemyState::MOVE, -1);
                           });


    enemyAnimator.AddEvent("animations/Enemy/Bat/BaseballBat_Attack3.csv",
                           14, [this]()
                           {
                               SetState(EnemyState::MOVE, -1);
                           });
}

void Bat::SetState(EnemyState Enemystate, int damageCount)
{
    Enemy::SetState(Enemystate, damageCount);
    switch (Enemystate)
    {
    case EnemyState::MOVE:
        if (health == 100 || health == 80)
        {
            enemyAnimator.Play(animationClipNames[0]);
        }
        else if (health == 60 || health == 40)
        {
            enemyAnimator.Play(animationClipNames[1]);
        }
        else
        {
            enemyAnimator.Play(animationClipNames[2]);
        }
        break;
    case EnemyState::ATTACK:
        if (health == 100 || health == 80)
        {
            enemyAnimator.Play(animationClipNames[3]);
        }
        else if (health == 60 || health == 40)
        {
            enemyAnimator.Play(animationClipNames[4]);
        }
        else
        {
            enemyAnimator.Play(animationClipNames[5]);
        }
        break;
    case EnemyState::DASH:
        if (health == 100 || health == 80)
        {
            enemyAnimator.Play(animationClipNames[6]);
        }
        else if (health == 60 || health == 40)
        {
            enemyAnimator.Play(animationClipNames[7]);
        }
        else
        {
            enemyAnimator.Play(animationClipNames[8]);
        }

        break;
    case EnemyState::HURT:
        if (health == 100 || health == 80)
        {
            enemyAnimator.Play(animationClipNames[9]);
        }
        else if (health == 60 || health == 40)
        {
            enemyAnimator.Play(animationClipNames[10]);
        }
        else
        {
            enemyAnimator.Play(animationClipNames[11]);
        }
        break;
    case EnemyState::DEAD:
        enemyAnimator.Play(animationClipNames[12]);
        break;
    case EnemyState::CATCHED:
        if (health == 100 || health == 80)
        {
            enemyAnimator.Play(animationClipNames[13]);
        }
        if (health == 60 || health == 40)
        {
            enemyAnimator.Play(animationClipNames[14]);
        }
        else
        {
            enemyAnimator.Play(animationClipNames[15]);
        }
        break;
    }
}
