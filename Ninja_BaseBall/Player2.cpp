#include "pch.h"
#include "Player2.h"
#include "ComboCommands.h"
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

void Player2::SetKickTimeOn()
{
	kickTimeOn = true;
	velocity.y = -600.f;
}

void Player2::SetBox(bool flip)
{
	if (flip)
	{
		attackBox.setOrigin({ 150.f, 100.f });
		grapBox.setOrigin({ 100.f, 100.f });
	}
	else
	{
		attackBox.setOrigin({ -120.f, 100.f });
		grapBox.setOrigin({ -70.f, 100.f });
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
	inputOn = false;
}

void Player2::DashAttack()
{
	dashAttackTimeOn = true;
	animator.Play("Animations/player/player_DashAttack.csv");
	inputOn = false;
}

void Player2::DynamiteKick()
{
	jumpY = GetPosition().y;
	kickTime = 1.f;
	animator.Play("Animations/player/player_DynamiteKick.csv");
	isGrounded = false;
	//inputOn = false;
}

void Player2::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);
	animatorEffect.SetTarget(&OnHitEffect);

	hasHitBox = true;

	attackBox.setFillColor(sf::Color::Red);
	grapBox.setFillColor(sf::Color::Blue);
	hitBox.setFillColor(sf::Color::Yellow);

	attackBox.setSize({ 40, 30 });
	grapBox.setSize({ 20,20 });
	hitBox.setSize({ 70,160 });

	attackBox.setOrigin({ -120.f, 150.f });
	grapBox.setOrigin({ -70.f, 150.f });
	hitBox.setOrigin({ 35.f, 190.f });

	playerShadow.SetTexture("graphics/2_Player/redShadow.png");
	playerShadow.SetOrigin({ 90.f, 35.f });

	float hp = maxHp;
}

void Player2::Reset()
{
	animator.ClearEvent();
	std::function<void()>AttackOn = std::bind(&Player2::SetAttackOn, this);
	std::function<void()>AttackOff = std::bind(&Player2::SetAttackOff, this);
	std::function<void()>KickOn = std::bind(&Player2::SetKickTimeOn, this);

	animator.AddEvent("Animations/player/player_Attack1.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack1.csv", 3, AttackOff);
	animator.AddEvent("Animations/player/player_Attack2.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack2.csv", 3, AttackOff);
	animator.AddEvent("Animations/player/player_Attack3.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack3.csv", 3, AttackOff);
	animator.AddEvent("Animations/player/player_Attack4.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_Attack4.csv", 3, AttackOff);

	animator.AddEvent("Animations/player/player_DashAttack.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_DashAttack.csv", 3, AttackOff);

	animator.AddEvent("Animations/player/player_JumpAttackSK.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_JumpAttackSK.csv", 3, AttackOff);

	animator.AddEvent("Animations/player/player_DynamiteKick.csv", 3, KickOn);

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

	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y);
	OnHitEffect.setScale({ 1.5f, 1.5f });

	combo = new ComboCommands();

	combo->SetCombo();

}

void Player2::Update(float dt)
{
	enemyHitBox = player->GetHitBox();

	//SpriteGo::Update(dt);
	animator.Update(dt);
	animatorEffect.Update(dt);

	float v = 0;
	float h = 0;

	if (inputOn)
	{

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

		//대시기능
		{
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
		}

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

	if (isGrounded && !isLeftDashing && !isRightDashing && !(InputManager::GetKey(sf::Keyboard::E)) && InputManager::GetKeyDown(sf::Keyboard::R))
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



	if (getHit && !hitTimeOn && !invincible)
	{
		player->isImpacted = true;
		inputOn = false;
		hitTimeOn = true;

		hp -= 25.f;

		if (hp <= 0.f)
		{
			isAlive = false;
		}

		invincible = true;
	}

	if (invincible)
	{
		invincibleTime -= dt;
		if (invincibleTime <= 0.f)
		{
			invincible = false;
			invincibleTime = 1.5f;
		}
	}

	//피격시
	{
		if (hitTimeOn)
		{
			if (!isAlive)
			{
				hitTime += dt;
				isGrounded = false;
				jumpY = GetPosition().y + 100.f;
				velocity.y = -800.f;
				jumpDirection = -(sprite.getScale().x);
				Death();
			}
			else
			{
				hitTime += dt;
				velocity.x = -(sprite.getScale().x) * 800.f;
				Bitted();

			}
		}

		if (!isAlive && isGrounded && animator.GetCurrentClipId() == "Animations/player/player_Damage3.csv")
		{
			animator.Play("Animations/player/player_Death2.csv");
			//inputOn = false;
		}

		if (hitTime > hitTimer)
		{
			hitTimeOn = false;
			hitTime = 0.f;
			inputOn = true;
			getHit = false;
		}


		//몬스터용 잡히기 테스트
		if (hitBox.getGlobalBounds().intersects(player->GetGrapBox()) && player->isGrip)
		{
			inputOn = false;
			SetPosition( { player->GetAttackBox().left, player->GetAttackBox().top + 70.f } );
			if (player->isAttack)
			{
				if (player->gripAttackCount == 3)
				{
					hitBox.setSize({ 0, 0 });
					player->isGrip = false;
					isAlive = false;
					hitTimeOn = true;
				}
				else
				{
					Bitted();
				}

			}

		}

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

	if (attackTimeOn)
	{
		attackTime -= dt;
	}

	////잡기박스와 닿았을때
	//if (!isGrip && (gripCoolTime == 0.f) && grapBox.getGlobalBounds().intersects(enemyHitBox))
	//{
	//	animator.Play("Animations/player/player_Grip.csv");
	//	isGrip = true;
	//	inputOn = false;
	//	attackTimeOn = true;
	//	attackTime = 2.f;
	//}


	if (gripCoolTime > 0.f)
	{
		gripCoolTime -= dt;
		if (gripCoolTime <= 0.f)
		{
			gripCoolTime = 0.f;
		}
	}




	//몬스터용 피격판정
	if (hitBox.getGlobalBounds().intersects(player->GetAttackBox()))
	{
		if (player->isAttack)
		{
			getHit = true;
		}

	}


	//공격박스와 닿았을때
	if (!isGrip && attackBox.getGlobalBounds().intersects(enemyHitBox))
	{

		if (InputManager::GetKeyDown(sf::Keyboard::E))
		{
			attackTimeOn = true;

			normalAttack += 1;
			switch (normalAttack)
			{
			case 1:
				animator.Play("Animations/player/player_Attack1.csv");
				attackTime = 0.5f;
				break;
			case 2:
				animator.Play("Animations/player/player_Attack2.csv");
				attackTime = 0.5f;
				break;
			case 3:
				animator.Play("Animations/player/player_Attack3.csv");
				attackTime = 0.5f;
				break;
			case 4:
				animator.Play("Animations/player/player_Attack4.csv");
				attackTime = 0.5f;
				normalAttack = 0;
				break;
			default:
				break;
			}
			player->OnDamage(20, 1, GetPosition().x);

		}
	}
	else
	{
		if (InputManager::GetKeyDown(sf::Keyboard::E))
		{
			animator.Play("Animations/player/player_Attack1.csv");

		}
	}

	//기술 모음
	{
		//점프중일때 기술
		if (!isGrounded)
		{
			if (InputManager::GetKeyDown(sf::Keyboard::E))
			{
				animator.Play("Animations/player/player_JumpAttackSK.csv"); //점프옆차기
				animator.PlayQueue("Animations/player/player_Jump.csv");
			}
		}

		//대시중일때 기술
		if (isGrounded && (isLeftDashing || isRightDashing))
		{
			if (InputManager::GetKeyDown(sf::Keyboard::E))
			{
				DashAttack();
				dashDirection = h;
			}
		}

		//잡기중일때 기술
		//if (isGrip && isGrounded)
		//{
		//	if (InputManager::GetKeyDown(sf::Keyboard::E))
		//	{
		//		animator.Play("Animations/player/player_GripAttack1.csv");
		//		attackTime = 2.f;
		//	}
		//}

		//다이너마이트 킥
		if (InputManager::GetKey(sf::Keyboard::E))
		{
			if (InputManager::GetKeyDown(sf::Keyboard::R))
			{
				inputOn = false;
				DynamiteKick();
			}
		}

	}


	//기술 움직임 처리
	if (dashAttackTimeOn)
	{
		velocity.x = dashDirection * 800.f;
		dashAttackTime -= dt;
	}

	if (dashAttackTime <= dashAttackTimer)
	{
		dashAttackTimeOn = false;
		animator.Play("Animations/player/player_Idle.csv");
		isLeftDashing = false;
		isRightDashing = false;
		dashAttackTime = 0.3f;
		inputOn = true;
	}

	if (kickTimeOn)
	{
		velocity.x = sprite.getScale().x * 500.f;
		kickTime -= dt;
	}

	if (kickTime <= kickTimer)
	{
		kickTimeOn = false;
		animator.Play("Animations/player/player_Idle.csv");
		kickTime = 1.f;
		inputOn = true;
	}


	position += velocity * dt;

	SetPosition(position);
	playerShadow.SetPosition(GetPosition());


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

	if (attackTimeOn && attackTime <= 0.f)
	{
		animator.PlayQueue("Animations/player/player_Idle.csv");
		attackTimeOn = false;
		inputOn = true;
		normalAttack = 0;

		isGrip = false;
		gripCoolTime = 2.f;
	}

	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });
	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y - 130);

}

void Player2::Draw(sf::RenderWindow& window)
{
	playerShadow.Draw(window);

	SpriteGo::Draw(window);

	window.draw(attackBox);
	window.draw(grapBox);
	window.draw(hitBox);
	window.draw(OnHitEffect, shader);

	if (SCENE_MANAGER.GetDeveloperMode())
	{
		attackBox.setFillColor(sf::Color::Red);
		grapBox.setFillColor(sf::Color::Blue);
		hitBox.setFillColor(sf::Color::Yellow);
	}
	else
	{
		attackBox.setFillColor(sf::Color::Transparent);
		grapBox.setFillColor(sf::Color::Transparent);
		hitBox.setFillColor(sf::Color::Transparent);
	}

}










