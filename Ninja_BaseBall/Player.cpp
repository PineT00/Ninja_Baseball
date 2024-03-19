#include "pch.h"
#include "Player.h"

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

	std::function<void()> funcInstance = std::bind(&Player::TestInstance, this);
	animator.AddEvent("animations/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player::TestStatic);
	animator.AddEvent("animations/Idle.csv", 5, funcStatic);

}

void Player::Reset()
{
	animator.ClearEvent();

	SetOrigin(Origins::BC);
	SetPosition({ 0.f, 300.f });
	animator.Play("animations/Idle.csv");
}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float h = InputManager::GetAxis(Axis::Horizontal);

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("animations/Jump.csv");
	}

	velocity.x = h * speed;

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
		//animator.Play("Animations/Idle.csv");
	}

	
}
