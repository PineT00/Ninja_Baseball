#include "pch.h"
#include "Player.h"
#include "ComboCommands.h"
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
		attackBox.setOrigin({ 150.f, 100.f });
		grapBox.setOrigin({ 100.f, 100.f });
	}
	else
	{
		attackBox.setOrigin({ -120.f, 100.f });
		grapBox.setOrigin({ -70.f, 100.f });
	}
}

void Player::Bitted()
{
	animatorEffect.Play("Animations/player/effect/player_OnHit.csv");
	animator.Play("Animations/player/player_Damage1.csv");
	animator.PlayQueue("Animations/player/player_Idle.csv");
}

void Player::Death()
{
	animatorEffect.Play("Animations/player/effect/player_OnHit.csv");
	animator.Play("Animations/player/player_Damage3.csv");
	inputOn = false;
}

void Player::DashAttack()
{
	animator.Play("Animations/player/player_DashAttack.csv");
	inputOn = false;
}

void Player::Init()
{
	SpriteGo::Init();

	animator.SetTarget(&sprite);
	animatorEffect.SetTarget(&OnHitEffect);

	hasHitBox = true;

	attackBox.setFillColor(sf::Color::Red);
	grapBox.setFillColor(sf::Color::Blue);
	hitBox.setFillColor(sf::Color::Yellow);
	
	attackBox.setSize({ 20, 20 });
	grapBox.setSize({ 20,20 });
	hitBox.setSize({ 70,90 });


	attackBox.setOrigin({ -120.f, 150.f });
	grapBox.setOrigin({ -70.f, 150.f });
	hitBox.setOrigin({ 35.f, 150.f });

	playerShadow.SetTexture("graphics/2_Player/redShadow.png");
	playerShadow.SetOrigin({90.f, 35.f});

	float hp = maxHp;
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

	//����ִϸ��̼�
	animator.Play("Animations/player/player_Spawn.csv");
	animator.PlayQueue("Animations/player/player_Idle.csv");
	SetOrigin(Origins::BC);


	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
	player2 = dynamic_cast<Player2*>(SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player2"));


	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });

	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y);
	OnHitEffect.setScale({1.5f, 1.5f});

	combo = new ComboCommands();

	combo->SetCombo();

}

void Player::Update(float dt)
{
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
			if (InputManager::GetKey(sf::Keyboard::Up))
			{
				v = -1;
			}
			else if (InputManager::GetKey(sf::Keyboard::Down))
			{
				v = 1;
			}
		}




		if (InputManager::GetKeyDown(sf::Keyboard::Right) && isGrounded)
		{
			rightDashReady = true;
		}
		if (InputManager::GetKeyDown(sf::Keyboard::Left) && isGrounded)
		{
			leftDashReady = true;
		}

		if (InputManager::GetKey(sf::Keyboard::Left) && isGrounded)
		{
			h = -1;
		}
		else if (InputManager::GetKey(sf::Keyboard::Right) && isGrounded)
		{
			h = 1;
		}

		//��ñ��
		{
			//���� ��� ����
			if (leftDashReady && leftDashTime > 0.f && leftDashTime < dashTimer)
			{
				if (InputManager::GetKeyDown(sf::Keyboard::Left))
				{
					isLeftDashing = true; // �뽬 ���� Ȱ��ȭ
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

			//������ ��� ����

			if (rightDashReady && rightDashTime > 0.f && rightDashTime < dashTimer)
			{
				if (InputManager::GetKeyDown(sf::Keyboard::Right))
				{
					isRightDashing = true; // �뽬 ���� Ȱ��ȭ
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
		}

	}


	//�뽬 ���ǵ�
	if (isRightDashing || isLeftDashing)
	{
		velocity.x = h * dashSpeed;
	}
	else
	{
		velocity.x = h * speed;
	}

	//����

	if (!isGrounded)
	{
		if (position.y >= jumpY)
		{
			isGrounded = true;
			SetPosition({ position.x, jumpY });
		}
	}

	if (isGrounded && !isLeftDashing && !isRightDashing && InputManager::GetKeyDown(sf::Keyboard::W))
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

	if (getHit)
	{
		getHit = false;
		inputOn = false;
		hitTimeOn = true;

		hp -= 25.f;

		if (hp <= 0.f)
		{
			isAlive = false;
		}

	}

	if (hitTimeOn)
	{
		if (!isAlive)
		{
			hitTime += dt;
			isGrounded = false;
			jumpY = GetPosition().y;
			Death();
			velocity.y = -800.f;
			jumpDirection = -(sprite.getScale().x);
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
	}

	if (hitTime > hitTimer)
	{
		hitTimeOn = false;
		hitTime = 0.f;
		inputOn = true;
	}


	if (dashAttackTimeOn)
	{
		velocity.x = dashDirection * 800.f;
		DashAttack();
		dashAttackTime -= dt;
	}
	if (dashAttackTime < dashAttackTimer)
	{
		dashAttackTimeOn = false;
		animator.Play("Animations/player/player_Idle.csv");
		isLeftDashing = false;
		isRightDashing = false;
		dashAttackTime = 0.3f;
		inputOn = true;
	}


	position += velocity * dt;

	SetPosition(position);
	playerShadow.SetPosition(GetPosition());

	if (isAttack)
	{
		velocity = { 0.f, 0.f };
		std::cout << isAttack << std::endl;
	}



	//�̵����� ����
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

	//���ݹڽ��� �������
	if (attackBox.getGlobalBounds().intersects(player2->GetHitBox()))
	{

		if (InputManager::GetKeyDown(sf::Keyboard::Q))
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
			player2->getHit;

		}
	}
	else
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Q))
		{
			animator.Play("Animations/player/player_Attack1.csv");
			
		}
	}

	//���ڽ��� �������
	if (grapBox.getGlobalBounds().intersects(player2->GetHitBox()))
	{
		isGrip = true;
		gripTime -= dt;
	}


	//��� ����
	{

		//�������϶� ���
		if (!isGrounded)
		{
			if (InputManager::GetKeyDown(sf::Keyboard::Q))
			{
				animator.Play("Animations/player/player_JumpAttackSK.csv"); //����������
				animator.PlayQueue("Animations/player/player_Jump.csv");
			}
		}

		//������϶� ���
		if (isGrounded && (isLeftDashing || isRightDashing))
		{
			if (InputManager::GetKeyDown(sf::Keyboard::Q))
			{
				dashDirection = h;
				dashAttackTimeOn = true;
			}
		}

		//������϶� ���
		if (isGrip && isGrounded)
		{
			if (InputManager::GetKeyDown(sf::Keyboard::Q))
			{
				animator.Play("Animations/player/player_GripAttack1.csv");
			}
		}

		//�޺� ��Ͽ�
		if (InputManager::GetKeyDown(sf::Keyboard::L))
		{
			InputManager::StopComboRecord();
			InputManager::ClearCombo();
			InputManager::ComboRecord(10.f);
		}

		if (InputManager::IsRecording() && InputManager::IsComboSuccess(*(combo->comboList[0])))
		{
			animator.Play("Animations/player/player_DashAttack.csv");
			InputManager::StopComboRecord();
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
	if (attackTimeOn && attackTime <= 0.f)
	{
		animator.PlayQueue("Animations/player/player_Idle.csv");
		attackTimeOn = false;
		normalAttack = 0;
	}


	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });
	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y - 130);


	//�ܻ�ȿ��
	trailDuration -= dt;

	if (trailDuration <= 0)
	{
		if (trails.size() < 3)
		{ // �ܻ��� �ִ� 3������ ����
			sf::Sprite trail;
			Animator trailAnimator;

			trailAnimator.SetTarget(&trail);
			trailAnimator.Play(animator.GetCurrentClipId(), animator.GetCurrentClipFrame());
			trail.setOrigin(GetOrigin().x + 150.f, GetOrigin().y + 250.f);
			trail.setColor(sf::Color(0, 0, 0, 100));
			trail.setPosition(GetPosition());
			if (h < 0.f || jumpDirection < 0.f)
			{
				trail.setScale(-1, 1);
			}
			else
			{
				trail.setScale(1, 1);
			}
			//trail.setTexture(texture);
			trails.push_back(trail);
		}
		else
		{
			trails.erase(trails.begin()); // ���� ������ �ܻ� ����
		}
		trailDuration = 0.05f; // �ܻ� ���� �ð� �ʱ�ȭ
	}
}

void Player::Draw(sf::RenderWindow& window)
{	
	playerShadow.Draw(window);

	//if (isLeftDashing || isRightDashing)
	//{
	//	for (const auto& trail : trails)
	//	{
	//		window.draw(trail, shader);
	//	}
	//}

	SpriteGo::Draw(window);

	window.draw(attackBox);
	window.draw(grapBox);
	window.draw(hitBox);
	window.draw(OnHitEffect,shader);

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










