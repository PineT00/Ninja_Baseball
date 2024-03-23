#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class ComboCommands;
class SceneDev1;
class Player2;

class Player :
    public SpriteGo
{
public:
	int normalAttack = 0;
	bool isAttack = false;

	//Turn THIS ON when monster Attacks Player.
	bool getHit = false;

	bool isAlive = true;
	float maxHp = 100.f;
	float hp = maxHp;
	float dmg = 20.f;

protected:
	ComboCommands* combo;
	SceneDev1* sceneDev1 = nullptr;
	Player2* player2 = nullptr;

	Animator animator;
	Animator animatorEffect;

	std::vector<sf::Sprite> trails;

	float trailDuration = 0.1f;

	float speed = 150.f;
	float dashSpeed = 400.f;

	float gravity = 1800.f;
	float jumpY = 0.f;
	int jumpDirection = 0;
	int dashDirection = 0;

	sf::Vector2f velocity;

	bool isGrounded = true;

	//Dash
	float dashTimer = 0.5f;

	float leftDashTime = 0.f;
	bool leftDashReady = false;
	bool isLeftDashing = false;

	float rightDashTime = 0.f;
	bool rightDashReady = false;
	bool isRightDashing = false;

	float dashAttackTimer = 0.f;
	float dashAttackTime = 0.3f;
	bool dashAttackTimeOn = false;

	//Hit
	float hitTimer = 0.15f;
	float hitTime = 0.f;
	bool hitTimeOn = false;

	//Attack Time
	float attackTimer = 0.f;
	float attackTime = 0.3f;
	bool attackTimeOn = false;

	//Grip Time
	bool isGrip = false;
	float gripTimer = 0.f;
	float gripTime = 2.f;
	float gripCoolTime = 1.f;

	//Turn Input on / off
	bool inputOn = true;

	SpriteGo playerShadow;
	sf::Sprite OnHitEffect;

	//충돌박스
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

	sf::FloatRect GetHitBox() { return hitBox.getGlobalBounds(); }

	void Bitted();
	void Death();

	//기술모음
	void DashAttack();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
	const sf::FloatRect& GetHitBox() const { return hitBox.getGlobalBounds(); }
};

