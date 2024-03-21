#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneDev1;
class Player2;

class Player :
    public SpriteGo
{
protected:
	SceneDev1* sceneDev1 = nullptr;
	Player2* player2 = nullptr;

	Animator animator;


	float speed = 150.f;
	float dashSpeed = 400.f;

	float gravity = 600.f;
	float jumpY = 0.f;

	sf::Vector2f velocity;

	bool isGrounded = true;




	float dashTimer = 0.5f;

	float leftDashTime = 0.f;
	bool leftDashReady = false;
	bool isLeftDashing = false;

	float rightDashTime = 0.f;
	bool rightDashReady = false;
	bool isRightDashing = false;

	int jumpDirection = 0;

	int normalAttack = 0;
	bool isAttack = false;

	sf::RectangleShape attackBox;
	sf::RectangleShape grapBox;
	sf::RectangleShape hitBox;

public:
	Player(const std::string& name = "");
	~Player();

	void TestInstance();
	static void TestStatic();

	void SetAttackOn();
	void SetAttackOff();
	void SetBox(bool flip);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

