#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class ComboCommands;
class SceneDev1;
class Player2;
class PickupItem;

class Player :
	public SpriteGo
{
public:
	enum class Status
	{
		isIdleWalk,
		isJumping,

		isDash,
		isDashAttack,
		isAttack,
		isKick,

		isGrip,

		isHitted,
		isKnockBack,
		isDead,

		isPickUp,
	};

	Status currStatus;

	int score = 0;
	int life = 1;

	int normalAttack = 0;
	bool isAttack = false;

	//Turn THIS ON when monster Attacks Player.
	bool getHit = false;

	bool isAlive = true;
	int maxHp = 100;
	int hp = maxHp;
	int dmg = 20;

	//impactTime
	bool isImpacted = false;
	float impactTimer = 0.15f;

	//Grip Time
	bool isGrip = false;
	float gripTime = 2.f;
	float gripCoolTime = 1.f;
	int gripAttackCount = 0;

	int hitWay = 0;

protected:

	Enemy* catchedEnemy = nullptr;
	ComboCommands* combo;
	SceneDev1* sceneDev1 = nullptr;
	PickupItem* pickupItem = nullptr;

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
	bool isJumping = false;
	bool isGrounded = true;

	//Dash
	float dashTimer = 1.0f;

	float leftDashTime = 0.f;
	bool leftDashReady = false;
	bool isLeftDashing = false;

	float rightDashTime = 0.f;
	bool rightDashReady = false;
	bool isRightDashing = false;


	//Dash Attack
	float dashAttackTime = 0.3f;
	bool dashAttackTimeOn = false;

	//Kick Attack
	float kickTime = 0.6f;
	bool kickTimeOn = false;

	//Attack Time
	float attackTime = 0.3f;
	bool attackTimeOn = false;


	//Hit
	float hitTime = 0.f;
	bool hitTimeOn = false;

	float invincibleTime = 1.5f;
	bool invincible = false;

	//Turn Input on / off
	bool inputOn = true;


	SpriteGo playerShadow;
	sf::Sprite OnHitEffect;
	sf::Sprite attackEffect;

	//충돌박스
	sf::RectangleShape attackBox;
	sf::RectangleShape grapBox;
	sf::RectangleShape hitBox;

	std::vector<Enemy*> enemyList;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	void SetAttackOn();
	void SetAttackOff();
	void SetKickTimeOn();

	void SetBox();
	void SetGripBox();

	sf::FloatRect GetHitBox() { return hitBox.getGlobalBounds(); }
	sf::FloatRect GetAttackBox() { return attackBox.getGlobalBounds(); }
	sf::FloatRect GetGrapBox() { return grapBox.getGlobalBounds(); }
	void ItemPickup(const std::string& itemName);

	void Bitted();
	void Death();

	//기술모음
	void DashAttack();

	void OnDamage(int damage, int type, float positionX);

	void DynamiteKick();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void UpdateIdle(float dt);
	void UpdateJumping(float dt);
	void UpdateDash(float dt);
	void UpdateDashAttack(float dt);
	void UpdateAttack(float dt);
	void UpdateKick(float dt);
	void UpdateGetHit(float dt);
	void UpdateGrip(float dt);
	void UpdateDead(float dt);
	void UpdatePickUp(float dt);

	void Draw(sf::RenderWindow& window) override;

	const sf::FloatRect& GetHitBox() const { return hitBox.getGlobalBounds(); }
	bool IsJumping() const { return isJumping; }
	bool IsInvincible() const { return invincible; }

	void SetStatus(Status newStatus);
};

