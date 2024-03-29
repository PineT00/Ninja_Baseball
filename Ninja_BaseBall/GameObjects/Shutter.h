#pragma once
#include "SpriteGo.h"

class SceneDev1;
class Player;
class Enemy;

class Shutter:public SpriteGo
{
public:
    enum class ShutterStatus
    {
        OPEN,
        CLOSED,
        DOWN,
        BREAK
    };
    ShutterStatus currStatus = ShutterStatus::OPEN;
    
    sf::FloatRect shutterRect;
    sf::RectangleShape shutterRectShape;
    sf::RectangleShape shutter;
    Animator animator;

    bool isBroken=false;
protected:
    SceneDev1* Scenedev1;
    Player* player;
    //Enemy* enemy;
    

    float WaitTimer=0.f;
    float shutterMoveSpeed = 100.f;
    
public:
    Shutter(const std::string& name);
    void Init() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void SetState(ShutterStatus state);
    void UpdateOpen(float dt);
    void UpdateClosed(float dt);
    void UpdateDown(float dt);
    void UpdateBreak(float dt);
};


