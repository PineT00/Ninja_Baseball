#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneDev1;

class Player :
    public SpriteGo
{
protected:
	Animator animator;

	float speed = 300.f;

	sf::Vector2f velocity;

	bool isGrounded = true;

	SceneDev1* sceneDev1 = nullptr;

public:
	Player(const std::string& name = "");
	~Player();

	void TestInstance();
	static void TestStatic();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};

