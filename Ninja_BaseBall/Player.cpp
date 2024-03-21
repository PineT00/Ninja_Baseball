#include "pch.h"
#include "Player.h"
#include "SceneDev1.h"

Player::Player(const std::string& name)
	:SpriteGo(name)
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

	attackBox.setFillColor(sf::Color::Red);
	grapBox.setFillColor(sf::Color::Red);
	hitBox.setFillColor(sf::Color::Yellow);
	
	attackBox.setSize({ 20,20 });
	grapBox.setSize({ 20,20 });
	hitBox.setSize({ 20,20 });


}

void Player::Reset()
{
	animator.ClearEvent();

	std::function<void()> funcInstance = std::bind(&Player::TestInstance, this);
	animator.AddEvent("Animations/player/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player::TestStatic);
	animator.AddEvent("Animations/player/player_Idle.csv", 5, funcStatic);
	SetPosition({ 0.f, 0.f });
	animator.Play("Animations/player/player_Idle.csv");
	SetOrigin(Origins::BC);

	//SetScale({ 0.75f, 0.75f });

	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());

	attackBox.setOrigin({30.f, 0.f});
	grapBox.setOrigin({ 10.f, 0.f });
	hitBox.setOrigin({ 0.f, 0.f });

}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	animator.Update(dt);

	float v = 0;
	if (isRightDashing || isLeftDashing)
	{
		v = 0;
	}
	else
	{

		if (InputManager::GetKey(sf::Keyboard::Up))
		{
			v = -1;
		}
		else if (InputManager::GetKey(sf::Keyboard::Down))
		{
			v = 1;
		}
	}


	float h = 0;

	if (InputManager::GetKeyDown(sf::Keyboard::Right))
	{
		rightDashReady = true;
	}
	if (InputManager::GetKeyDown(sf::Keyboard::Left))
	{
		leftDashReady = true;
	}

	if (InputManager::GetKey(sf::Keyboard::Left))
	{
		h = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::Right))
	{
		h = 1;
	}

	//왼쪽 대시 모음
	if (leftDashReady && leftDashTime > 0.f && leftDashTime < dashTimer)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Left))
		{
			isLeftDashing = true; // 대쉬 상태 활성화
			animator.Play("Animations/player/player_Dash.csv");
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
	if (InputManager::GetKeyUp(sf::Keyboard::Left))
	{
		isLeftDashing = false;
	}

	//오른쪽 대시 모음

	if (rightDashReady && rightDashTime > 0.f && rightDashTime < dashTimer)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Right))
		{
			isRightDashing = true; // 대쉬 상태 활성화
			animator.Play("Animations/player/player_Dash.csv");
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
	if (InputManager::GetKeyUp(sf::Keyboard::Right))
	{
		isRightDashing = false;
	}



	if (isRightDashing || isLeftDashing)
	{
		velocity.x = h * dashSpeed;
	}
	else
	{
		velocity.x = h * speed;
	}



	if (isGrounded == false)
	{
		velocity.y += gravity * dt;
		if (jumpDirection != 0.f)
		{
			velocity.x = jumpDirection * speed;
		}
		else
		{
			velocity.x = 0.f;
		}
	}
	else
	{
		velocity.y = v * speed;
	}

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		jumpY = GetPosition().y;
		animator.Play("Animations/player/player_Jump.csv");
		velocity.y = -300.f;
		jumpDirection = h;

	}


	position += velocity * dt;

	if (!isGrounded)
	{


		if (position.y >= jumpY)
		{
			isGrounded = true;
			SetPosition({ position.x, jumpY });
		}
	}

	if ((sceneDev1 != nullptr) && isGrounded)
	{
		position = sceneDev1->ClampByTileMap(position);
	}

	SetPosition(position);

	if (h != 0.f)
	{
		SetFlipX(h < 0);
	}

	//콤보용 기술 모음
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Q))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::W))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::E))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::R))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::T))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::Y))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
		if (InputManager::GetKeyDown(sf::Keyboard::U))
		{
			animator.Play("Animations/player/player_Jump.csv");
		}
	}


	if (animator.GetCurrentClipId() == "Animations/player/player_Idle.csv")
	{
		if (h != 0.f || v != 0.f)
		{
			animator.Play("Animations/player/player_Walk.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Animations/player/player_Walk.csv")
	{
		if (h == 0.f && v == 0.f)
		{
			animator.Play("Animations/player/player_Idle.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "Animations/player/player_Jump.csv" && isGrounded)
	{
		animator.Play("Animations/player/player_Idle.csv");
	}
	else if (animator.GetCurrentClipId() == "Animations/player/player_Dash.csv" && !isLeftDashing && !isRightDashing)
	{
		animator.Play("Animations/player/player_Idle.csv");
	}
}

void Player::draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	window.draw(attackBox);
	window.draw(grapBox);
	window.draw(hitBox);


}









