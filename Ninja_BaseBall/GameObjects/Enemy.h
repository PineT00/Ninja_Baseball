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
    bool isDead = false;

protected:
    //맞는 이펙트 추가
    Animator enemyEffectAnimator;
    sf::Sprite enemyOnHit;


    SceneDev1* scene;
    Player* player;
    Animator enemyAnimator;

    bool isCatch=false;

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
    float hurtTimer=0.f;
    float hurtDuration=1.f;
    float deadTimer=0.f;
    float deadDuration=2.f;

    float normalAttackDistance = 175.f;
    float dashMaxDistance=300.f;
    sf::Vector2f dashDirection;
    float attackDirection;
    int damage=10;
    sf::Vector2f dashEndPosition;
    
    bool isAttackPlay = false;

    int randX = 0;

    int damageCount;

    int health = 100;
    int maxHealth=100;
    sf::Vector2f catchedPosition;
public:
    Enemy(const std::string& name) : SpriteGo(name), scene(nullptr), player(nullptr)
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
    virtual void HoldAction();
};
