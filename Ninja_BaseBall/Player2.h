#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneDev1;

class Player2 :
	public SpriteGo
{
protected:
	Animator animator;

	float speed = 100.f;
	float dashSpeed = 200.f;

	sf::Vector2f velocity;

	bool isGrounded = true;

	SceneDev1* sceneDev1 = nullptr;

	float dashTimer = 1.5f;

	float leftDashTime = 0.f;
	bool leftDashReady = false;
	bool isLeftDashing = false;

	float rightDashTime = 0.f;
	bool rightDashReady = false;
	bool isRightDashing = false;

	sf::RectangleShape hitBox;


public:
	Player2(const std::string& name = "");
	~Player2();

	void TestInstance();
	static void TestStatic();

	sf::FloatRect GetHitBox() { return hitBox.getGlobalBounds(); }

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

