#include "pch.h"
#include "Player2.h"
#include "SceneDev1.h"

Player2::Player2(const std::string& name)
	:SpriteGo(name)
{
}

Player2::~Player2()
{
}

void Player2::TestInstance()
{
	std::cout << "TestInstance()" << std::endl;
}

void Player2::TestStatic()
{
	std::cout << "TestStatic()" << std::endl;
}

void Player2::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);

	hasHitBox = true;
}

void Player2::Reset()
{
	animator.ClearEvent();

	std::function<void()> funcInstance = std::bind(&Player2::TestInstance, this);
	animator.AddEvent("Animations/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player2::TestStatic);
	animator.AddEvent("Animations/Idle.csv", 5, funcStatic);
	SetPosition({ 0.f, 0.f });
	animator.Play("Animations/Idle.csv");
	SetOrigin(Origins::BC);

	SetScale({ 0.75f, 0.75f });

	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());

}

void Player2::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float v = 0;
	if (InputManager::GetKey(sf::Keyboard::I))
	{
		v = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::K))
	{
		v = 1;
	}



	float h = 0;

	if (InputManager::GetKeyDown(sf::Keyboard::L))
	{
		rightDashReady = true;
	}
	if (InputManager::GetKeyDown(sf::Keyboard::J))
	{
		leftDashReady = true;
	}

	if (InputManager::GetKey(sf::Keyboard::J))
	{
		h = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::L))
	{
		h = 1;
	}


	//왼쪽 대시 모음
	if (leftDashReady && leftDashTime > 0.f && leftDashTime < dashTimer)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::L))
		{
			isLeftDashing = true; // 대쉬 상태 활성화
		}
	}
	if (leftDashTime > dashTimer)
	{
		leftDashTime = 0.f;
		leftDashReady = false;
	}
	if (leftDashReady)
	{
		leftDashTime += dt;
	}
	if (InputManager::GetKeyUp(sf::Keyboard::J))
	{
		isLeftDashing = false;
	}

	//오른쪽 대시 모음

	if (rightDashReady && rightDashTime > 0.f && rightDashTime < dashTimer)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::L))
		{
			isRightDashing = true; // 대쉬 상태 활성화
		}
	}
	if (rightDashTime > dashTimer)
	{
		rightDashTime = 0.f;
		rightDashReady = false;
	}
	if (rightDashReady)
	{
		rightDashTime += dt;
	}
	if (InputManager::GetKeyUp(sf::Keyboard::L))
	{
		isRightDashing = false;
	}

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("Animations/Jump.csv");
	}

	if (isRightDashing || isLeftDashing)
	{
		velocity.x = h * dashSpeed;
	}
	else
	{
		velocity.x = h * speed;
	}

	velocity.y = v * speed;

	position += velocity * dt;

	if (sceneDev1 != nullptr)
	{
		position = sceneDev1->ClampByTileMap(position);
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









