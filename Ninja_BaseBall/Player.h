#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneDev1;

class Player :
    public SpriteGo
{
protected:
	Animator animator;

	float speed = 100.f;
	float dashSpeed = 200.f;

	float gravity = 600.f;
	float jumpY = 0.f;

	sf::Vector2f velocity;

	bool isGrounded = true;

	SceneDev1* sceneDev1 = nullptr;

	float dashTimer = 0.5f;

	float leftDashTime = 0.f;
	bool leftDashReady = false;
	bool isLeftDashing = false;

	float rightDashTime = 0.f;
	bool rightDashReady = false;
	bool isRightDashing = false;

	int jumpDirection = 0;

	int normalAttack = 0;

	sf::RectangleShape attackBox;
	sf::RectangleShape grapBox;
	sf::RectangleShape hitBox;

public:
	Player(const std::string& name = "");
	~Player();

	void TestInstance();
	static void TestStatic();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void draw(sf::RenderWindow& window);
};

