#include "pch.h"
#include "Player.h"
#include "SceneDev1.h"
#include "Player2.h"

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

void Player::SetAttackOn()
{
	isAttack = true;
}

void Player::SetAttackOff()
{
	isAttack = false;
	isLeftDashing = false;
	isRightDashing = false;
}

void Player::SetBox(bool flip)
{
	if (flip)
	{
		attackBox.setOrigin({ 150.f, 150.f });
		grapBox.setOrigin({ 100.f, 150.f });
	}
	else
	{
		attackBox.setOrigin({ -120.f, 150.f });
		grapBox.setOrigin({ -70.f, 150.f });
	}
}

void Player::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);

	hasHitBox = true;

	attackBox.setFillColor(sf::Color::Red);
	grapBox.setFillColor(sf::Color::Blue);
	hitBox.setFillColor(sf::Color::Yellow);
	
	attackBox.setSize({ 20, 20 });
	grapBox.setSize({ 20,20 });
	hitBox.setSize({ 50,50 });


	attackBox.setOrigin({ -120.f, 150.f });
	grapBox.setOrigin({ -70.f, 150.f });
	hitBox.setOrigin({ 20.f, 150.f });


}

void Player::Reset()
{
	animator.ClearEvent();
	std::function<void()>AttackOn = std::bind(&Player::SetAttackOn, this);
	std::function<void()>AttackOff = std::bind(&Player::SetAttackOff, this);
	animator.AddEvent("Animations/player/player_Attack1.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack1.csv", 3, AttackOff);

	animator.AddEvent("Animations/player/player_DashAttack.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_DashAttack.csv", 3, AttackOff);


	std::function<void()> funcInstance = std::bind(&Player::TestInstance, this);
	animator.AddEvent("Animations/player/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player::TestStatic);
	animator.AddEvent("Animations/player/player_Idle.csv", 5, funcStatic);
	SetPosition({ 0.f, 0.f });
	animator.Play("Animations/player/player_Idle.csv");
	SetOrigin(Origins::BC);


	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
	player2 = dynamic_cast<Player2*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player2"));


	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });

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

	if (InputManager::GetKeyDown(sf::Keyboard::Right) && isGrounded)
	{
		rightDashReady = true;
	}
	if (InputManager::GetKeyDown(sf::Keyboard::Left) && isGrounded)
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


	//대쉬 스피드
	if (isRightDashing || isLeftDashing)
	{
		velocity.x = h * dashSpeed;
	}
	else
	{
		velocity.x = h * speed;
	}

	//점프

	if (!isGrounded)
	{
		if (position.y >= jumpY)
		{
			isGrounded = true;
			SetPosition({ position.x, jumpY });
		}
	}

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		jumpY = GetPosition().y;
		animator.Play("Animations/player/player_Jump.csv");
		velocity.y = -300.f;
		jumpDirection = h;
	}

	if (!isGrounded)
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

	position += velocity * dt;
	SetPosition(position);

	if (isAttack)
	{
		velocity = { 0.f, 0.f };
		std::cout << isAttack << std::endl;
	}



	//이동영역 제한
	if ((sceneDev1 != nullptr) && isGrounded)
	{
		position = sceneDev1->ClampByTileMap(position);
	}


	if (h != 0.f)
	{
		SetFlipX(h < 0);
		SetBox(h < 0);
	}

	if (attackBox.getGlobalBounds().intersects(player2->GetHitBox()))
	{

		if (InputManager::GetKeyDown(sf::Keyboard::Q))
		{
			normalAttack += 1;
			switch (normalAttack)
			{
				case 1:
					animator.Play("Animations/player/player_Attack1.csv");
					break;
				case 2:
					animator.Play("Animations/player/player_Attack2.csv");
					break;
				case 3:
					animator.Play("Animations/player/player_Attack3.csv");
					break;
				case 4:
					animator.Play("Animations/player/player_Attack4.csv");
					normalAttack = 0;
					break;
				default:
					break;
			}
		}
	}
	else
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Q))
		{
			animator.Play("Animations/player/player_Attack1.csv");
		}
	}


	//콤보용 기술 모음
	{
		if (isLeftDashing || isRightDashing)
		{
			if (InputManager::GetKeyDown(sf::Keyboard::W))
			{
				animator.Play("Animations/player/player_DashAttack.csv");
			}
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

	if (!isAttack && animator.GetCurrentClipId() == "Animations/player/player_Attack1.csv")
	{
		animator.PlayQueue("Animations/player/player_Idle.csv");
	}
	if (!isAttack && animator.GetCurrentClipId() == "Animations/player/player_Attack4.csv")
	{
		animator.PlayQueue("Animations/player/player_Idle.csv");
	}


	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });
}

void Player::Draw(sf::RenderWindow& window)
{	
	SpriteGo::Draw(window);

	if (SCENE_MANAGER.GetDeveloperMode())
	{
		window.draw(attackBox);
		window.draw(grapBox);
		window.draw(hitBox);
	}

}










