#pragma once
#include "Enemy.h"

class Player;

class YellowBaseBall:public Enemy
{
public:
    Player* player = nullptr;

protected:
    enum class YellowBaseBallState
    {
        IDLE,
        MOVE,
        ATTACK,
        HURT,
        DEAD
    };
    YellowBaseBallState currentState = YellowBaseBallState::IDLE;
    
    Animator yellowBaseBallAnimator;
    
    bool hasMovedInitial = false;
    float initialMoveDistance = 100.f;
public:
    YellowBaseBall(const std::string& name);
    void Init() override;
    void Release() override;
    void Reset() override;
    void FixedUpdate(float dt) override;
    void LateUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void Update(float dt) override;
    void OnDamage(int damage) override;
    
};
