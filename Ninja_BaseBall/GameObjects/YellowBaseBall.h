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
    float attackTimer = 0.f;
    float attackCooldown = 2.f;
    float prepareAttackDistance = 30.f;
    float retreatDistance = 10.f;
    float prepareAttackTimer = 3.f;
    float prepareAttackDuration = 1.f;
    float attackDistance = 5.f;
    float acceptableYDistance = 15.f;

    int hitCount=0;
    int maxHitCount=3;
    sf::Clock hitClock;
protected:
    enum class YellowBaseBallState
    {
        INTRO,
        MOVE,
        DASH,
        PREPARE_ATTACK,
        ATTACK,
        RETREAT,
        CATCH,
        HURT,
        DEAD,
        WAIT
    };
    YellowBaseBallState currentState = YellowBaseBallState::INTRO;
    
    Animator yellowBaseBallAnimator;
    SceneDev1* sceneDev1 = nullptr;
    sf::FloatRect playerBounds;
    
    sf::FloatRect damageBounds;
    sf::FloatRect attackBounds;

    //hitBox는 플레이어에게 맞을 곳 attackBox는 플레이어를 때릴 곳
    sf::RectangleShape damageBox;
    sf::RectangleShape attackBox;
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
    static sf::Vector2f Normalize(const sf::Vector2f& source);
    void GetCurrentAnimationName();
    void SetPosition(const sf::Vector2f& pos) override;
    void DrawBox();
    void Direction(const sf::Vector2f& playerPosition);
};
