#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player :
    public SpriteGo
{
protected:
	Animator animator;

	//float gravity = 500.f;

	float speed = 300.f;
	sf::Vector2f velocity;

	bool isGrounded = true;

public:
	Player(const std::string& name = "");
	~Player();

	void TestInstance();
	static void TestStatic();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};

