#pragma once
#include "Enemy.h"

class SceneDev1;
class Player;

class YellowBaseBall:public Enemy
{
public :
    //잡힘 여부 판단 
    //bool isCatched = false;
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

    bool attackCoolOn = false;

    int hitCount=0;
    int maxHitCount=3;
    
    //int maxHealth =200;
    //int health = maxHealth;
    sf::Clock hitClock;

    float updateInterval=0.1f;
    float updateTimer=0.f;

    float deadTimer = 2.f;
    
protected:
    SceneDev1* sceneDev1 = nullptr;
    sf::FloatRect playerBounds;
    
    sf::FloatRect damageBounds;
    sf::FloatRect attackBounds;

    //damageBox 플레이어에게 맞을 곳 ,attackBox 플레이어를 때릴 곳
    //sf::RectangleShape damageBox;
    //sf::RectangleShape attackBox;

    sf::Vector2f playerPosition;
public:
    Player* player;

    void SetState(int damageCount=0);
    YellowBaseBall(const std::string& name);
    void Init() override;
    void Release() override;
    void Reset() override;
    void FixedUpdate(float dt) override;
    void LateUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void Update(float dt) override;
    void OnDamage(int damage,int count) override;
    void TargetDirection(const sf::Vector2f& playerPosition) override;
    void Attack() override;
    void DashToPlayer(float dt,sf::Vector2f& currentPosition) override;
    void NormalMovement(float dt, sf::Vector2f& currentPosition, const sf::Vector2f& playerPosition, float xDistance, float yDistance) override;
    void StartDash(const sf::Vector2f& playerPosition, const sf::Vector2f& currentPosition) override;
    sf::FloatRect GetHitBox() const override;
    void Damage() override;
};
