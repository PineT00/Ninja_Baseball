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

    float updateInterval=0.1f;
    float updateTimer=0.f;
    
protected:
    enum class YellowBaseBallState
    {
        INTRO,
        MOVE,
        DASH,
        ATTACK,
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

    //damageBox 플레이어에게 맞을 곳 ,attackBox 플레이어를 때릴 곳
    sf::RectangleShape damageBox;
    sf::RectangleShape attackBox;

    sf::Vector2f playerPosition;
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
    void TargetDirection(const sf::Vector2f& playerPosition) override;
    void Attack() override;
    void DashToPlayer(float dt,sf::Vector2f& currentPosition) override;
    void NormalMovement(float dt, sf::Vector2f& currentPosition, const sf::Vector2f& playerPosition, float xDistance, float yDistance) override;
    void StartDash(const sf::Vector2f& playerPosition, const sf::Vector2f& currentPosition) override;
};
