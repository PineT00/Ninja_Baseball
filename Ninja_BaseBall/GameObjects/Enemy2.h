#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class SceneDev1;
class Player;

class Enemy2:public SpriteGo
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
public:
    Enemy2(const std::string& name) : SpriteGo(name), Scene(nullptr), player(nullptr)
    {
    }

    void SetState(EnemyState Enemystate);
    void Init() override;
    void UpdateIdle(float dt);
    void UpdateMove(float dt);
    void UpdateAttack(float dt);
    void UpdateDash(float dt);
    void UpdateHurt(float dt);
    void UpdateDead(float dt);
    void UpdateCatched(float dt);
    void Update(float dt) override;
    void Release() override;
    void Reset() override;
    void Draw(sf::RenderWindow& window) override;
    void SetPosition(const sf::Vector2f& pos) override;
    void SetFlipX(bool flip) override;
    
};
