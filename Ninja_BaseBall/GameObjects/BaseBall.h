#pragma once
#include "Enemy.h"

class SceneDev1;

class BaseBall:public Enemy
{
public:
   enum class BaseBallColor
   {
       YELLOW,
       WHITE,
       BLUE,
       GREEN,
   };
    BaseBall(BaseBallColor color);
    static BaseBall* Create(BaseBallColor color);
protected:
    
   void SetState(EnemyState Enemystate, int damageCount) override;

   SceneDev1* Scenedev1;

   std::vector<std::string >animationClipNames;
    std::string animationClipPath;
private:
    BaseBallColor color;
    

    
};
