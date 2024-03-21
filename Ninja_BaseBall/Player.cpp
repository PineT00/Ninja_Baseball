#include "pch.h"
#include "Player.h"
#include "SceneDev1.h"

Player::Player(const std::string& name):SpriteGo(name)
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

	SetScale({ 0.75f, 0.75f });

	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());

}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float h = InputManager::GetAxis(Axis::Horizontal);
	float v = InputManager::GetAxis(Axis::Vertical);

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("Animations/Jump.csv");
	}

	velocity.x = h * speed;
	velocity.y = v * speed;

	position += velocity * dt;


	if (sceneDev1 != nullptr)
	{
		position = sceneDev1->ClampByTileMap(position);
	}
	if(velocity != sf::Vector2f(0,0))
	{
		facingDirection=velocity/std::sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	}
	SetPosition(position);
	//std::cout << position.x << std::endl;

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

sf::Vector2f Player::GetBackPosition() const
{
	return facingDirection;
}













