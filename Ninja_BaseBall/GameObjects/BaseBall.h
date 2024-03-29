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
    BaseBall(BaseBallColor color,  const std::string& name);
    static BaseBall* Create(BaseBallColor color,const std::string& stageName);
protected:
   void Init() override; 
   void SetState(EnemyState Enemystate, int damageCount) override;


   SceneDev1* Scenedev1;

   std::vector<std::string >animationClipNames;

private:
   BaseBallColor color;
    

    
};
