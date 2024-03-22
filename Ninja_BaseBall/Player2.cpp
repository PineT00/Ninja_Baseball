#include "pch.h"
#include "Player2.h"
#include "SceneDev1.h"
#include "Player.h"

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
void Player2::SetAttackOn()
{
	isAttack = true;
}

void Player2::SetAttackOff()
{
	isAttack = false;
	isLeftDashing = false;
	isRightDashing = false;
}

void Player2::SetBox(bool flip)
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

void Player2::Bitted()
{
	animatorEffect.Play("Animations/player/effect/player_OnHit.csv");
	animator.Play("Animations/player/player_Damage1.csv");
	animator.PlayQueue("Animations/player/player_Idle.csv");
}

void Player2::Death()
{
	animatorEffect.Play("Animations/player/effect/player_OnHit.csv");
	animator.Play("Animations/player/player_Damage3.csv");
	//inputOn = false;
}

void Player2::DashAttack()
{
	velocity.x + 800.f;
	animator.Play("Animations/player/player_DashAttack.csv");
}

void Player2::Init()
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

	playerShadow.SetTexture("graphics/2_Player/redShadow.png");
	playerShadow.SetOrigin({ 90.f, 35.f });
}

void Player2::Reset()
{
	animator.ClearEvent();
	std::function<void()>AttackOn = std::bind(&Player2::SetAttackOn, this);
	std::function<void()>AttackOff = std::bind(&Player2::SetAttackOff, this);
	animator.AddEvent("Animations/player/player_Attack1.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack1.csv", 3, AttackOff);

	animator.AddEvent("Animations/player/player_DashAttack.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_DashAttack.csv", 3, AttackOff);


	std::function<void()> funcInstance = std::bind(&Player2::TestInstance, this);
	animator.AddEvent("Animations/player/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&Player2::TestStatic);
	animator.AddEvent("Animations/player/player_Idle.csv", 5, funcStatic);

	//등장애니메이션
	animator.Play("Animations/player/player_Spawn.csv");
	animator.PlayQueue("Animations/player/player_Idle.csv");
	SetOrigin(Origins::BC);


	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"));


	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });

}

void Player2::Update(float dt)
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
		if (InputManager::GetKey(sf::Keyboard::I))
		{
			v = -1;
		}
		else if (InputManager::GetKey(sf::Keyboard::K))
		{
			v = 1;
		}
	}


	float h = 0;

	if (InputManager::GetKeyDown(sf::Keyboard::L) && isGrounded)
	{
		rightDashReady = true;
	}
	if (InputManager::GetKeyDown(sf::Keyboard::J) && isGrounded)
	{
		leftDashReady = true;
	}

	if (InputManager::GetKey(sf::Keyboard::J) && isGrounded)
	{
		h = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::L) && isGrounded)
	{
		h = 1;
	}

	//왼쪽 대시 모음
	if (leftDashReady && leftDashTime > 0.f && leftDashTime < dashTimer)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::J))
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
	if (InputManager::GetKeyUp(sf::Keyboard::L))
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

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::M))
	{
		isGrounded = false;
		jumpY = GetPosition().y;
		animator.Play("Animations/player/player_Jump.csv");
		velocity.y = -800.f;
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
	playerShadow.SetPosition(GetPosition());

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

	if (attackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{

		if (InputManager::GetKeyDown(sf::Keyboard::U))
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

			player->getHit = true;
		}
	}
	else
	{
		if (InputManager::GetKeyDown(sf::Keyboard::U))
		{
			animator.Play("Animations/player/player_Attack1.csv");
		}
	}


	//기술 모음
	{

		//점프중일때 기술
		if (!isGrounded)
		{

		}

		//잡기중일때 기술
		if (isHolding)
		{

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

void Player2::Draw(sf::RenderWindow& window)
{
	playerShadow.Draw(window);

	SpriteGo::Draw(window);

	if (SCENE_MANAGER.GetDeveloperMode())
	{
		window.draw(attackBox);
		window.draw(grapBox);
		window.draw(hitBox);
	}

}





