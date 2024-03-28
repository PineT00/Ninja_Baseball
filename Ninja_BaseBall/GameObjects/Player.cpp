#include "pch.h"
#include "Player.h"
#include "ComboCommands.h"
#include "SceneDev1.h"
#include "WindyPlane.h"
#include "PickupItem.h"

Player::Player(const std::string& name)
	: SpriteGo(name), combo(nullptr)
{
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

void Player::SetKickTimeOn()
{
	velocity.y = -600.f;
	jumpY = GetPosition().y;
	isGrounded = false;
	kickTimeOn = true;
}

void Player::SetBox()
{
	if (sprite.getScale().x < 0)
	{
		attackBox.setOrigin({ 175.f, 100.f });
		grapBox.setOrigin({ 100.f, 100.f });
	}
	else
	{
		attackBox.setOrigin({ -120.f, 100.f });
		grapBox.setOrigin({ -70.f, 100.f });
	}
}

void Player::SetGripBox()
{
	if (sprite.getScale().x < 0)
	{
		attackBox.setOrigin({ 120.f, 100.f });
	}
	else
	{
		attackBox.setOrigin({ -90.f, 100.f });
	}
}

void Player::ItemPickup(const std::string& itemName)
{
	pickupItem = dynamic_cast<PickupItem*>(sceneDev1->FindGameObject(itemName));

	pickupItem->SetActive(false);
	animator.PlayQueue("Animations/player/player_GetGoldBat.csv");
	stageClear = true;
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
}

void Player::DashAttack()
{
	dashAttackTimeOn = true;
	animator.Play("Animations/player/player_DashAttack.csv");
}


void Player::OnDamage(int damage, int type, float positionX)
{
	if (!hitTimeOn)
	{
		getHit = true;
		hp -= damage;

		if (positionX > GetPosition().x)
		{
			hitWay = -1;
		}
		else
		{
			hitWay = 1;
		}
	}
}

void Player::DynamiteKick()
{
	kickTime = 0.7f;
	animator.Play("Animations/player/player_DynamiteKick.csv");

	//inputOn = false;

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

	attackBox.setSize({ 50, 80 });
	grapBox.setSize({ 20,20 });
	hitBox.setSize({ 70,160 });

	attackBox.setOrigin({ -120.f, 100.f });
	grapBox.setOrigin({ -70.f, 100.f });
	hitBox.setOrigin({ 35.f, 190.f });

	playerShadow.SetTexture("graphics/2_Player/redShadow.png");
	playerShadow.SetOrigin({ 90.f, 35.f });

	animator.ClearEvent();
	std::function<void()>AttackOn = std::bind(&Player::SetAttackOn, this);
	std::function<void()>AttackOff = std::bind(&Player::SetAttackOff, this);
	std::function<void()>KickOn = std::bind(&Player::SetKickTimeOn, this);
	std::function<void()>GripAttackOn = std::bind(&Player::SetGripBox, this);
	std::function<void()>GripAttackOff = std::bind(&Player::SetBox, this);

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
	animator.AddEvent("Animations/player/player_DynamiteKick.csv", 2, AttackOn);
	animator.AddEvent("Animations/player/player_DynamiteKick.csv", 20, AttackOff);

	animator.AddEvent("Animations/player/player_GripAttack1.csv", 1, AttackOn);
	animator.AddEvent("Animations/player/player_GripAttack1.csv", 4, GripAttackOff);
	animator.AddEvent("Animations/player/player_GripAttack1.csv", 4, AttackOff);
}

void Player::Reset()
{
	SetSortLayer(0);

	SetActive(true);
	hp = maxHp;
	//등장애니메이션
	animator.Play("Animations/player/player_Spawn.csv");
	animator.PlayQueue("Animations/player/player_Idle.csv");
	SetOrigin(Origins::BC);

	sceneDev1 = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
	pickupItem = dynamic_cast<PickupItem*>(sceneDev1->FindGameObject("goldbat"));

	if (life == 0)
	{
		SetPosition({ sceneDev1->worldViewCenter.x, sceneDev1->worldViewCenter.y + 100.f });
	}

	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });

	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y);
	OnHitEffect.setScale({ 2.f, 2.f });

	isAlive = true;

	combo = new ComboCommands();

	combo->SetCombo();

	getHit = false;
	stageClear = false;
	stageClearTimer = 0.f;
	SetStatus(Status::isIdleWalk);
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
	animatorEffect.Update(dt);

	attackBox.setPosition({ GetPosition() });
	grapBox.setPosition({ GetPosition() });
	hitBox.setPosition({ GetPosition() });

	OnHitEffect.setPosition(hitBox.getPosition().x, hitBox.getPosition().y - 180.f);

	if (stageClear)
	{
		stageClearTimer += dt;
		if (stageClearTimer > stageClearInterval)
		{
			SCENE_MANAGER.ChangeScene(SceneIDs::SceneTitle);
		}
	}

	if (getHit)
	{
		hitTimeOn = true;
		if (hp > 0.f)
		{
			hitTime = 0.2f;
			SetStatus(Status::isHitted);
		}
		else
		{
			hitTime = 2.5f;
			jumpY = GetPosition().y;
			velocity.y = -800.f;
			Death();
			SetStatus(Status::isDead);
		}

	}
	if (!isGrip)
	{
		catchedEnemy = nullptr;
		if (gripCoolTime > 0.f)
		{
			gripCoolTime -= dt;
		}
	}


	switch (currStatus)
	{
	case Status::isIdleWalk:
		UpdateIdle(dt);
		break;
	case Status::isJumping:
		UpdateJumping(dt);
		break;
	case Status::isDash:
		UpdateDash(dt);
		break;
	case Status::isDashAttack:
		UpdateDashAttack(dt);
		break;
	case Status::isAttack:
		UpdateAttack(dt);
		break;
	case Status::isKick:
		UpdateKick(dt);
		break;
	case Status::isGrip:
		UpdateGrip(dt);
		break;
	case Status::isHitted:
		UpdateGetHit(dt);
		break;
	case Status::isDead:
		UpdateDead(dt);
		break;
	case Status::isPickUp:
		UpdatePickUp(dt);
		break;

	default:
		break;
	}


}

void Player::UpdateIdle(float dt)
{
	float v = 0;
	float h = 0;

	if (!leftDashReady && InputManager::GetKeyDown(sf::Keyboard::Left))
	{
		leftDashTime = 0.3f;
	}
	if (!rightDashReady && InputManager::GetKeyDown(sf::Keyboard::Right))
	{
		rightDashTime = 0.3f;
	}
	if (leftDashTime >= 0.f)
	{
		leftDashTime -= dt;
	}
	if (rightDashTime >= 0.f)
	{
		rightDashTime -= dt;
	}

	if (InputManager::GetKey(sf::Keyboard::Up))
	{
		v = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::Down))
	{
		v = 1;
	}

	if (InputManager::GetKey(sf::Keyboard::Left))
	{
		h = -1;
	}
	else if (InputManager::GetKey(sf::Keyboard::Right))
	{
		h = 1;
	}

	if (!leftDashReady && InputManager::GetKeyUp(sf::Keyboard::Left))
	{
		leftDashReady = true; // 대쉬 대기
	}
	if (!rightDashReady && InputManager::GetKeyUp(sf::Keyboard::Right))
	{
		rightDashReady = true; // 대쉬 대기
	}

	//왼쪽 대시 준비
	if (leftDashReady && leftDashTime > 0.f)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Left))
		{
			leftDashReady = false;
			isLeftDashing = true; // 대쉬 상태 활성화
			dashDirection = h;
			animator.Play("Animations/player/player_Dash.csv");
			SetStatus(Status::isDash);
		}
	}
	if (leftDashTime <= 0.f)
	{
		leftDashReady = false;
	}
	//오른쪽 대시 준비

	if (rightDashReady && rightDashTime > 0.f)
	{
		if (InputManager::GetKeyDown(sf::Keyboard::Right))
		{
			rightDashReady = false;
			isRightDashing = true; // 대쉬 상태 활성화
			dashDirection = h;
			animator.Play("Animations/player/player_Dash.csv");
			SetStatus(Status::isDash);
		}
	}
	if (rightDashTime <= 0.f)
	{
		rightDashReady = false;
	}

	if (h != 0.f)
	{
		SetFlipX(h < 0);
		SetBox();
	}

	velocity.x = h * speed;
	velocity.y = v * speed;

	position += velocity * dt;

	SetPosition(position);


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
	if (animator.GetCurrentClipId() == "Animations/player/player_Jump.csv")
	{
		animator.Play("Animations/player/player_Idle.csv");
	}

	enemyList = sceneDev1->GetEnemyList();

	for (auto& enemy : enemyList)
	{
		if (enemy == nullptr) continue;

		if (!enemy->GetActive()) continue;

		if (isGrip) continue;

		if (gripCoolTime > 0.f) continue;

		if (grapBox.getGlobalBounds().intersects(enemy->GetDamageBox()) && !enemy->isDead)
		{
			animator.Play("Animations/player/player_Grip.csv");
			isGrip = true;
			enemy->HoldAction();
			catchedEnemy = enemy;
			gripTime = 2.f;
			SetStatus(Status::isGrip);
		}
	}

	if (attackTimeOn)
	{
		attackTime -= dt;

		if (attackTime <= 0.f)
		{
			animator.Play("Animations/player/player_Idle.csv");
			attackTimeOn = false;
			normalAttack = 0;
		}
	}
	if (InputManager::GetKeyDown(sf::Keyboard::W))
	{
		if (InputManager::GetKey(sf::Keyboard::Q))
		{
			attackTimeOn = false;
			normalAttack = 0;
			DynamiteKick();
			SetStatus(Status::isKick);
		}
		else
		{
			jumpY = GetPosition().y;
			velocity.y = -800.f;
			jumpDirection = h;
			animator.Play("Animations/player/player_Jump.csv");
			SetStatus(Status::isJumping);

		}
	}


	if (InputManager::GetKeyDown(sf::Keyboard::Q))
	{
		{
			if (Utils::MyMath::Distance(pickupItem->GetPosition(), position) < 50)
			{
				pickupItem->GetPickUpAction()("goldbat");
				pickupItem->SetIsPicked(true);
				SetStatus(Status::isPickUp);
			}
			else
			{
				SetStatus(Status::isAttack);
			}
		}
	}
}

void Player::UpdateJumping(float dt)
{
	if (position.y > jumpY)
	{
		SetPosition({ position.x, jumpY });
		SetStatus(Status::isIdleWalk);
	}
	else
	{
		velocity.y += gravity * dt;
	}

	if (jumpDirection != 0.f)
	{
		velocity.x = jumpDirection * speed;
	}
	else
	{
		velocity.x = 0.f;
	}

	if (InputManager::GetKeyDown(sf::Keyboard::Q))
	{
		enemyList = sceneDev1->GetEnemyList();

		for (auto& enemy : enemyList)
		{
			if (enemy == nullptr) continue;
			if (attackBox.getGlobalBounds().intersects(enemy->GetDamageBox()))
			{
				enemy->OnDamage(20, normalAttack);
			}
		}
		animator.Play("Animations/player/player_JumpAttackSK.csv"); //점프옆차기
		animator.PlayQueue("Animations/player/player_Jump.csv");
	}

	position += velocity * dt;
	SetPosition(position);
}

void Player::UpdateDash(float dt)
{
	if (InputManager::GetKeyUp(sf::Keyboard::Left))
	{
		isLeftDashing = false;

	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Right))
	{
		isRightDashing = false;

	}

	if (!isLeftDashing && !isRightDashing)
	{
		rightDashReady = false;
		leftDashReady = false;
		animator.Play("Animations/player/player_Idle.csv");
		SetStatus(Status::isIdleWalk);
	}
	else
	{
		velocity.x = dashDirection * dashSpeed;
		position.x += velocity.x * dt;
	}

	SetPosition(position);

	if (InputManager::GetKeyUp(sf::Keyboard::Q))
	{
		isLeftDashing = false;
		isRightDashing = false;
		animator.Play("Animations/player/player_DashAttack.csv");
		SetStatus(Status::isDashAttack);
	}
}

void Player::UpdateDashAttack(float dt)
{
	enemyList = sceneDev1->GetEnemyList();

	for (auto& enemy : enemyList)
	{
		if (enemy == nullptr) continue;
		if (attackBox.getGlobalBounds().intersects(enemy->GetDamageBox()))
		{
			enemy->OnDamage(20, normalAttack);
		}
	}

	if (dashAttackTime > 0.f)
	{
		velocity.x = dashDirection * 800.f;
		position.x += velocity.x * dt;
		dashAttackTime -= dt;
	}
	else
	{
		dashAttackTime = 0.3f;
		animator.Play("Animations/player/player_Idle.csv");
		SetStatus(Status::isIdleWalk);
	}

	SetPosition(position);
}

void Player::UpdateAttack(float dt)
{
	enemyList = sceneDev1->GetEnemyList();
	bool demageDealt = false;
	for (auto& enemy : enemyList)
	{
		if (enemy == nullptr) continue;
		//isAttack&&
		if (attackBox.getGlobalBounds().intersects(enemy->GetDamageBox()) && !enemy->isDead)
		{
			enemy->OnDamage(20, normalAttack);
			score += 10;

			if (!demageDealt)
			{
				normalAttack += 1;
			}
			demageDealt = true;
		}
	}

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
		animator.Play("Animations/player/player_Attack1.csv");
		normalAttack = 0;
		break;
	}

	attackTimeOn = true;
	attackTime = 0.3f;

	std::cout << normalAttack << std::endl;

	SetStatus(Status::isIdleWalk);
}

void Player::UpdateKick(float dt)
{

	enemyList = sceneDev1->GetEnemyList();

	for (auto& enemy : enemyList)
	{
		if (enemy == nullptr) continue;
		//isAttack&&
		if (isAttack && attackBox.getGlobalBounds().intersects(enemy->GetDamageBox()))
		{
			enemy->OnDamage(20, 5);
			isAttack = false;
		}
	}

	if (kickTimeOn)
	{
		velocity.x = sprite.getScale().x * 700.f;
		velocity.y += gravity * dt;
		kickTime -= dt;
	}

	if (kickTime <= 0.f)
	{
		kickTimeOn = false;
		animator.Play("Animations/player/player_Idle.csv");
		kickTime = 0.7f;
		SetStatus(Status::isIdleWalk);
	}
	position += velocity * dt;
	SetPosition(position);

}

void Player::UpdateGrip(float dt)
{
	if (animator.GetCurrentClipId() != "Animations/player/player_GripAttack1.csv"
		&& InputManager::GetKeyDown(sf::Keyboard::Q))
	{
		SetGripBox();
		animator.Play("Animations/player/player_GripAttack1.csv");
		animator.PlayQueue("Animations/player/player_Grip.csv");
		catchedEnemy->OnDamage(20, 0);
		gripAttackCount += 1;
		gripTime = 2.f;
	}
	gripTime -= dt;

	if (gripTime <= 0.f)
	{
		gripCoolTime = 1.f;
		isGrip = false;
		gripAttackCount = 0;
		animator.Play("Animations/player/player_Idle.csv");
		gripTime = 2.f;
		catchedEnemy = nullptr;
		SetStatus(Status::isIdleWalk);
	}

	if (gripAttackCount >= 3)
	{
		gripCoolTime = 1.f;
		isGrip = false;
		gripAttackCount = 0;
		animator.Play("Animations/player/player_Idle.csv");
		gripTime = 2.f;
		catchedEnemy = nullptr;
		SetStatus(Status::isIdleWalk);
	}

	std::cout << gripAttackCount << std::endl;

}

void Player::UpdateGetHit(float dt)
{
	gripCoolTime = 1.f;
	isGrip = false;
	if (getHit)
	{
		velocity.x = hitWay * 800.f;
		Bitted();
	}
	else
	{
		hitTime -= dt;
	}
	getHit = false;

	position.x += velocity.x * dt;
	SetPosition(position);

	if (hitTime <= 0.f)
	{
		hitTimeOn = false;
		hitTime = 0.2f;
		animator.Play("Animations/player/player_Idle.csv");
		SetStatus(Status::isIdleWalk);
	}
}

void Player::UpdateDead(float dt)
{
	isGrip = false;
	if (getHit)
	{
		Death();
	}
	else
	{
		hitTime -= dt;
	}
	getHit = false;

	if (position.y > jumpY)
	{
		animator.Play("Animations/player/player_Death2.csv");
		velocity.x = 0.f;
		velocity.y = 0.f;
		SetPosition({ position.x, jumpY });
	}

	if (animator.GetCurrentClipId() == "Animations/player/player_Damage3.csv")
	{
		velocity.x = hitWay * speed;
		velocity.y += gravity * dt;
	}

	if (hitTime <= 0.f)
	{
		hitTimeOn = false;
		hitTime = 0.2f;
		if (life == 1)
		{
			life = 0;
			Reset();
		}
		else
		{
			SetActive(false);
		}
	}



	position += velocity * dt;
	SetPosition(position);
	/////
}

void Player::UpdatePickUp(float dt)
{
	if (animator.GetCurrentClipId() != "Animations/player/player_GetGoldBat.csv")
	{
		animator.Play("Animations/player/player_GetGoldBat.csv");
	}

	if (animator.GetCurrentClipFrame() == animator.GetCurrentClip()->GetTotalFrame() - 1)
	{
		SetStatus(Status::isIdleWalk);
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	playerShadow.Draw(window);

	if (kickTimeOn)
	{
		for (const auto& trail : trails)
		{
			window.draw(trail, shader);
		}
	}

	SpriteGo::Draw(window);

	window.draw(attackBox);
	window.draw(grapBox);
	window.draw(hitBox);
	window.draw(OnHitEffect, shader);

	if (SCENE_MANAGER.GetDeveloperMode())
	{
		grapBox.setFillColor(sf::Color::Blue);
		hitBox.setFillColor(sf::Color::Yellow);
		attackBox.setFillColor(sf::Color::Red);
	}
	else
	{
		grapBox.setFillColor(sf::Color::Transparent);
		hitBox.setFillColor(sf::Color::Transparent);
		attackBox.setFillColor(sf::Color::Transparent);
	}
}



void Player::SetStatus(Status newStatus)
{
	Status prevStatus = currStatus;
	currStatus = newStatus;

	switch (currStatus)
	{
	case Status::isIdleWalk:
		//mainScreen->SetActive(true);
		//selectScreen->SetActive(false);
		break;
	case Status::isJumping:
		//mainScreen->SetActive(false);
		//selectScreen->SetActive(true);
		break;
	case Status::isDash:
		//mainScreen->SetActive(false);
		//selectScreen->SetActive(false);
		break;
	case Status::isDashAttack:
		break;
	case Status::isAttack:
		break;

	}

}
