#include "pch.h"
#include "Player.h"
#include "SceneDev1.h"

Player::Player(const std::string& name)
{
}

Player::~Player()
{
}

void Player::TestInstance()
{
	std::cout << "TestInstance()" << std::endl;
}

void Player::TestStatic()
{
	std::cout << "TestStatic()" << std::endl;
}

void Player::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);

	hasHitBox = true;
}

void Player::Reset()
{
	animator.ClearEvent();

	std::function<void()> funcInstance = std::bind(&Player::TestInstance, this);
	animator.AddEvent("Animations/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player::TestStatic);
	animator.AddEvent("Animations/Idle.csv", 5, funcStatic);
	SetPosition({ 0.f, 0.f });
	animator.Play("Animations/Idle.csv");
	SetOrigin(Origins::BC);

}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float h = InputManager::GetAxis(Axis::Horizontal);

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("Animations/Jump.csv");
		velocity.y = -500.f;
	}

	velocity.x = h * speed;
	velocity.y += gravity * dt;

	position += velocity * dt;

	if (position.y > 0.f)
	{
		isGrounded = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}
	SetPosition(position);

	if (h != 0.f)
	{
		SetFlipX(h < 0);
	}

	if (animator.GetCurrentClipId() == "Animations/Idle.csv")
	{
		if (h != 0.f)
		{
			animator.Play("Animations/Run.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Animations/Run.csv")
	{
		if (h == 0.f)
		{
			animator.Play("Animations/Idle.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Animations/Jump.csv" && isGrounded)
	{
		animator.Play("Animations/Idle.csv");
	}

}




	//if (sceneDev1 != nullptr)
	//{
	//	//position = sceneDev1->ClampByTileMap(position);
	//}




