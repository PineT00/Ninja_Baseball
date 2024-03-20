#pragma once
#include "SpriteGo.h"
#include "TestScene.h"
#include "Animator.h"

class TestPlayer:public SpriteGo
{
protected:
    int health = 100;
    int maxHealth = 100;
    int damage = 10;
    float speed = 100.0f;
    float jumpForce = 100.0f;
    float attackRange = 100.0f;
    float attackCooldown = 1.0f;
    Animator playerAnimator;
public:
    TestScene* testScene;
    YellowBaseBall* enemy;
    TestPlayer(const std::string& name);
    
    void Init() override;
    void Reset() override;
    void Update(float dt) override;
    void OnDamage(int damage);
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Attack();
    void Draw(sf::RenderWindow& window) override;
    
};
