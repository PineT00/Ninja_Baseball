#pragma once
#include "Enemy.h"

class SceneDev1;
class Player;

class YellowBaseBall:public Enemy
{
public :
    //잡힘 여부 판단 
    bool isCatch = false;
    //이동 여부 판단
    bool hasMovedInitial = false;
    //공격 관련
    float attackDistance = 10.f;
    float attackDelay = 1.f;
    float attackTimer = 0.f;
    float attackCooldown = 2.f;
    float prepareAttackDistance = 30.f;
    float minDistance = 10.f;
    float retreatDistance = 10.f;
    float dashSpeed = 10.f;
    
    float acceptableYDistance = 15.f;
protected:
    enum class YellowBaseBallState
    {
        IDLE,
        MOVE,
        DASH,
        PREPARE_ATTACK,
        ATTACK,
        RETREAT,
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
    void DashTowards(const sf::Vector2f& target,float dt);
    void Attack();
    void RetreatAfterAction();
    void MoveTowards(const sf::Vector2f& target, float speed, float dt);
    sf::Vector2f Normalize(const sf::Vector2f& source);
};
