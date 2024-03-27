#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class SceneDev1;
class Player;

class Enemy:public SpriteGo
{
public:
    enum class EnemyState
    {
        //IDLE,
        MOVE,
        ATTACK,
        DASH,
        HURT,
        DEAD,
        CATCHED,
    };
    EnemyState Enemystate=EnemyState::MOVE;
    enum EnemyType
    {
        NONE,
        NORMAL,
        BOSS,
    };
    EnemyType enemyType=EnemyType::NONE;
protected:
    SceneDev1* Scene;
    Player* player;
    Animator enemyAnimator;


    sf::Vector2f playerPos;

    sf::RectangleShape damageBox;
    sf::RectangleShape attackBox;
    float minDistance = 2000.f;
    float acceptableYDistance = 15.f;
    float acceptableXDistance = 170.f;
    float speed = 100.f;
    float dashSpeed = 400.f;

    float attackTimer = 0.f;
    float attackCooldown = 2.f;
    float dashTimer = 0.f;
    float dashCooldown = 3.f;

    float normalAttackDistance = 170.f;
    float dashMaxDistance=300.f;
    sf::Vector2f dashDirection;
    int damage=10;
    sf::Vector2f dashEndPosition;
    
    bool isAttackPlay = false;
    bool isDead = false;

    int damageCount;

    int health = 100;
    int maxHealth=100;
    sf::Vector2f catchedPosition;
public:
    Enemy(const std::string& name) : SpriteGo(name), Scene(nullptr), player(nullptr)
    {
    }

    virtual void SetState(EnemyState Enemystate,int damageCount=1);
    void Init() override;
    virtual void UpdateIdle(float dt);
    virtual void UpdateMove(float dt);
    virtual void UpdateAttack(float dt);
    virtual void UpdateDash(float dt);
    virtual void UpdateHurt(float dt);
    virtual void UpdateDead(float dt);
    virtual void UpdateCatched(float dt);
    void Update(float dt) override;
    void Release() override;
    void Reset() override;
    void Draw(sf::RenderWindow& window) override;
    void SetPosition(const sf::Vector2f& pos) override;
    void SetFlipX(bool flip) override;

    virtual sf::FloatRect GetDamageBox() const;
    virtual void OnDamage(int damage, int count);
};
