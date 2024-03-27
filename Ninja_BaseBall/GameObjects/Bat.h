#pragma once

class SceneDev1;

class Bat:public Enemy
{
public:
    enum class BatColor
    {
        YELLOW,
        //WHITE,
        //BLUE,
        //GREEN,
    };
    Bat(BatColor color,const std::string& name);
    static Bat* Create(BatColor color,const std::string& stageName);
protected:
    void Init() override;
    void SetState(EnemyState Enemystate, int damageCount) override;

    SceneDev1* Scenedev1;

    std::vector<std::string>animationClipNames;
    
    
private:
    BatColor color;
};
