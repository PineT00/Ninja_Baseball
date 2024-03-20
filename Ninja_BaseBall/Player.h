#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneDev1;

class Player :
    public SpriteGo
{
protected:
	Animator animator;

	float speed = 50.f;
	float dashSpeed = 300.f;

	sf::Vector2f velocity;

	bool isGrounded = true;

	SceneDev1* sceneDev1 = nullptr;

	float leftDashTime = 0.f;
	float dashTimer = 1.5f;
	bool dashReady = false;
	bool isDashing = false;

	float RightDashTime = 0.f;
	bool dashReady = false;
	bool isDashing = false;


public:
	Player(const std::string& name = "");
	~Player();

	void TestInstance();
	static void TestStatic();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};

