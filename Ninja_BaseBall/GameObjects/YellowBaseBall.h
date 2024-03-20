#pragma once
#include "Enemy.h"

class SceneDev1;
class Player;

class YellowBaseBall:public Enemy
{
public :
    bool isCatch = false;
    bool hasMovedInitial = false;
    float initialMoveDistance = 10.f;
    float followDistance = 30.f;
    float attackDistance = 10.f;
protected:
    enum class YellowBaseBallState
    {
        IDLE,
        MOVE,
        DASH,
        ATTACK,
        HURT,
        DEAD
    };
    YellowBaseBallState currentState = YellowBaseBallState::IDLE;
    
    Animator yellowBaseBallAnimator;
    SceneDev1* sceneDev1 = nullptr;
    
public:
    Player* player;
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
