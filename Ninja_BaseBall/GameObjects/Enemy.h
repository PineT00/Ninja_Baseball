#pragma once
#include "Animator.h"
#include "SpriteGo.h"

class SceneDev1;
class Player;

class Enemy:public SpriteGo
{
protected:
    SceneDev1* Scene = nullptr;
    Player* player = nullptr;
    Animator enemyAnimator;

    float speed = 100.f;
    int health = 100;
    int maxHealth = 100;
    int damage = 10;
    bool isDead = false;
    bool isAttacking = false;
    
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
};
