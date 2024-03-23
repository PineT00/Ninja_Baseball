#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class SceneDev1;
class Player;

class Enemy:public SpriteGo
{
public :
    enum class EnemyState
    {
        IDLE,
        MOVE,
        ATTACK,
        DASH,
        HURT,
        DEAD,
        CATCH,
    };
    EnemyState currentEnemy=EnemyState::IDLE;
    sf::Vector2f lastPosition;
protected:
    SceneDev1* Scene;
    Player* player;
    Animator enemyAnimator;

    float speed = 150.f;
    int health = 100;
    int maxHealth = 100;
    int damage = 10;
    
    bool isDead = false;
    bool isAttack = false;
    bool isReadyToDash=false;
    bool isDash =false;
    bool isWaiting = false;

    float waitTimer=0.f;
    float waitDuration=0.5f;

    float dashDuration = 0.5f;
    float dashTimer = 0.0f;
    float dashSpeed = 300.f;
    float dashCooldown = 3.f;
    float dashCooldownTimer = 0.f;
    float acceptableYDistance = 15.f;
    float minDashDistance = 30.f;
    float dashDistance = 50.f;
    float maxDistance = 500.f;
    float minDistance=200.f;

    float attackTimer = 0.f;
    float attackCooldown = 2.f;
    float ySpeedIncreaseFactor = 1.5f;
    
    sf::FloatRect playerBounds;
    sf::FloatRect damageBounds;
    sf::FloatRect attackBounds;
    sf::FloatRect playerHitBox;

    sf::RectangleShape damageBox;
    sf::RectangleShape attackBox;
    sf::Vector2f attackBoxSize = { 20, 20 };
    sf::Vector2f playerPos;
public:
    Enemy(const std::string& name);
    
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    virtual void OnDamage(int damage);
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    float GetHealth() const { return health; }
    void SetPosition(const sf::Vector2f& position) override
    {
        SpriteGo::SetPosition(position);
        //sprite.setPosition(position);
    }

    void DashTowards(const sf::Vector2f& target, float dt);
    virtual void Attack();

    void MoveTowards(const sf::Vector2f& target, float dt);
    static sf::Vector2f Normalize(const sf::Vector2f& source);
    virtual void TargetDirection(const sf::Vector2f& playerPosition);
    virtual void SetBox(bool flip);

    void SetPlayerHitBox(const sf::FloatRect& hitBox) { playerHitBox = hitBox; }
    bool CheckHitBox() const { return attackBox.getGlobalBounds().intersects(playerHitBox); }
    virtual void UpdateDashState(float dt);
    virtual void DashToPlayer(float dt);
    virtual void MoveToPlayer(float dt);
    virtual void MoveToPlayerDiagon(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition);
    virtual void MoveToPlayerX(float dt, const sf::Vector2f& targetPosition, const sf::Vector2f& currentPosition);
    
};
