#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class ComboCommands;
class SceneDev1;
class Player;

//임시 몬스터

class Player2 :
	public SpriteGo
{
protected:
	ComboCommands* combo;
	SceneDev1* sceneDev1 = nullptr;
	Player* player = nullptr;

	Animator animator;

	std::vector<sf::Sprite> trails;

	float trailDuration = 0.1f;

	float speed = 150.f;
	float dashSpeed = 400.f;

	float gravity = 1300.f;
	float jumpY = 0.f;

	sf::Vector2f velocity;

	bool isGrounded = true;
	bool isHolding = false;



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


	SpriteGo playerShadow;


	sf::RectangleShape attackBox;
	sf::RectangleShape grapBox;
	sf::RectangleShape hitBox;

public:
	Player2(const std::string& name = "");
	~Player2();

	void TestInstance();
	static void TestStatic();

	void SetAttackOn();
	void SetAttackOff();
	void SetBox(bool flip);

	sf::FloatRect GetHitBox() { return hitBox.getGlobalBounds(); }

	//기술모음
	void DashAttack();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};

