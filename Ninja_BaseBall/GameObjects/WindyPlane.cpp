#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"
#include "SceneDevBoss.h"
#include "rapidcsv.h"

WindyPlane::WindyPlane(const std::string& name)
	: SpriteGo(name), data(BOSS_TABLE->Get(BossType::WINDYPLANE))
{
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);
	effectAnimator.SetTarget(&spriteEffect);

	// Hit, Attack Box
	attackBox.setFillColor(sf::Color::Red);
	attackBox.setSize({ 400,80 }); 
	attackBox.setOrigin({ 0.f, 150.f });

	rangedAttackBox.setFillColor(sf::Color::Blue);
	rangedAttackBox.setSize({ 400, 120 });
	rangedAttackBox.setOrigin({ -300.f, 100.f });

	hitBox.setFillColor(sf::Color::Yellow);
	hitBox.setSize({ 140,160 });
	hitBox.setOrigin({ 35.f, 150.f });


	// 데이터 테이블로 가져온 클립 추가
	clipInfos.resize(PARTS_STATUS_COUNT + 1);

	for (int parts = 0; parts < PARTS_STATUS_COUNT; ++parts)
	{
		clipInfos[parts].partsStatus = static_cast<BossPartsStatus>(parts);

		for (int clipCount = 0; clipCount < data[clipInfos[parts].partsStatus].files.size(); ++clipCount)
		{
			clipInfos[parts].clips.push_back(data[clipInfos[parts].partsStatus].files[clipCount]); // 클립 추가
			bool status = parts == clipCount ? true : false;
			clipInfos[parts].clipStatus.push_back(status);
		}
	}

	// clipInfos[parts] : 상태별 모음
	// clipInfos[parts].clips : 상태별 클립 모음
	// clipInfos[parts].clipStatus : 상태별 클립 사용 여부

	//Wing Event
	std::function<void()> onDamagedEvent = std::bind(&WindyPlane::OnDamagedEvent, this);
	animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::DAMAGED], 3, onDamagedEvent);

	std::function<void()> attackOneTwo = std::bind(&WindyPlane::AttackOneTwoEvent, this);
	for (int i = 12; i <= 15; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::ONETWO], i, attackOneTwo);
	}

	std::function<void()> attackStraight = std::bind(&WindyPlane::AttackStraightEvent, this);
	for (int i = 19; i <= 30; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::STRAIGHT], i, attackStraight);
	}

	std::function<void()> attackUpperCut = std::bind(&WindyPlane::AttackUpperCutEvent, this);
	for (int i = 5; i <= 8; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::UPPERCUT], i, attackUpperCut);
	}

	std::function<void()> attackWind = std::bind(&WindyPlane::AttackWindEvent, this);
	for (int i = 0; i <= 5; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::WIND], i, attackWind);
	}

	std::function<void()> attackGun = std::bind(&WindyPlane::AttackGunEvent, this);
	for (int i = 0; i <= 5; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[(int)WindyPlaneStatus::GUN], i, attackGun);
	}



	//NoWing Event
	animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::DAMAGED], 4, onDamagedEvent);

	for (int i = 6; i <= 14; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::ONETWO], i, attackOneTwo);
	}

	for (int i = 8; i <= 12; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::STRAIGHT], i, attackStraight);
	}

	for (int i = 5; i <= 8; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::UPPERCUT], i, attackUpperCut);
	}

	std::function<void()> attackGunReady = std::bind(&WindyPlane::AttackGunReadyEvent, this);
	for (int i = 0; i <= 12; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::GUNREADY], i, attackGunReady);
	}
	
	for (int i = 0; i <= 5; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoWing].clips[(int)WindyPlaneStatus::GUN], i, attackGun);
	}



	//NoProp Event
	animator.AddEvent(clipInfos[(int)BossPartsStatus::NoProp].clips[(int)WindyPlaneStatus::DAMAGED], 4, onDamagedEvent);

	for (int i = 6; i <= 15; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoProp].clips[(int)WindyPlaneStatus::ONETWO], i, attackOneTwo);
	}

	for (int i = 8; i <= 12; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoProp].clips[(int)WindyPlaneStatus::STRAIGHT], i, attackStraight);
	}

	for (int i = 5; i <= 8; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::NoProp].clips[(int)WindyPlaneStatus::UPPERCUT], i, attackUpperCut);
	}

	//OneArm Event
	animator.AddEvent(clipInfos[(int)BossPartsStatus::OneArm].clips[(int)WindyPlaneStatus::DAMAGED], 4, onDamagedEvent);

	for (int i = 8; i <= 12; ++i)
	{
		animator.AddEvent(clipInfos[(int)BossPartsStatus::OneArm].clips[(int)WindyPlaneStatus::STRAIGHT], i, attackStraight);
	}


	//NoArm Event
	animator.AddEvent(clipInfos[(int)BossPartsStatus::NoArm].clips[(int)WindyPlaneStatus::DAMAGED], 4, onDamagedEvent);

	std::function<void()> deathEvent = std::bind(&WindyPlane::OnDieEvent, this);
	animator.AddEvent(clipInfos[(int)BossPartsStatus::OneArm].clips[(int)WindyPlaneStatus::STRAIGHT], 6, deathEvent);


	// FindGo
	sceneDevBoss = dynamic_cast<SceneDevBoss*>(SCENE_MANAGER.GetScene(SceneIDs::SceneDevBoss));
	player = dynamic_cast<Player*>(sceneDevBoss->FindGameObject("player"));
}

void WindyPlane::Reset()
{
	SetOrigin(Origins::BC);
}

void WindyPlane::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

	attackBox.setPosition(position);
	rangedAttackBox.setPosition(position);
	hitBox.setPosition(position);

	// HP 마다 상태가 바뀐다.
	if (InputManager::GetKeyDown(sf::Keyboard::Num1))
	{
		hp = hp - maxHp * 0.2f;
		std::cout << hp << std::endl;
	}

	if (InputManager::GetKeyDown(sf::Keyboard::Numpad1))
	{
		currentStatus = WindyPlaneStatus::DAMAGED;
	}

	if (InputManager::GetKeyDown(sf::Keyboard::Numpad2))
	{
		currentStatus = WindyPlaneStatus::GUN;
	}

	if (InputManager::GetKeyDown(sf::Keyboard::Numpad3))
	{
		currentStatus = WindyPlaneStatus::WIND;

		//UpdateWindAttack(dt);
	}

	switch (currentPartsStatus)
	{
	case BossPartsStatus::Wing:
	case BossPartsStatus::NoWing:
	case BossPartsStatus::NoProp:
	{
		if (attackBox.getGlobalBounds().intersects(player->GetHitBox()))
		{
			statusTimer += dt;

			if (statusTimer >= statusInterval)
			{
				float dist = Utils::MyMath::Distance(player->GetPosition(), position);
				std::cout << dist << std::endl;

				statusTimer = 0.f;
				hitCount = 0;

				if (dist <= 200)
				{
					// OneTwo
					currentStatus = WindyPlaneStatus::UPPERCUT;
				}
				else if (dist <= 400)
				{
					currentStatus = WindyPlaneStatus::ONETWO;
				}
				else if (dist <= 600)
				{
					currentStatus = WindyPlaneStatus::STRAIGHT;
				}
			}
		}
	}
		break;
	case BossPartsStatus::OneArm:
		break;
	case BossPartsStatus::NoArm:
		break;
	case BossPartsStatus::UNDEFINED:
		break;
	default:
		break;
	}

	ChasePlayer(dt);
	PlayAnimation(currentPartsStatus, currentStatus);
}

void WindyPlane::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	window.draw(attackBox);
	window.draw(rangedAttackBox);
	window.draw(hitBox);
}

void WindyPlane::SetFlipX(bool flipX)
{
	SpriteGo::SetFlipX(flipX);

	if (flipX)
	{
		attackBox.setScale({ scale.x, scale.y });
		rangedAttackBox.setScale({ scale.x, scale.y });
		hitBox.setScale({ scale.x, scale.y });
	}
	else
	{
		attackBox.setScale({ -scale.x, scale.y });
		rangedAttackBox.setScale({ -scale.x, scale.y });
		hitBox.setScale({ -scale.x, scale.y });
	}
}

void WindyPlane::ChasePlayer(float dt)
{
	findTimer += dt;

	if (findTimer >= findInterval)
	{
		findTimer = 0.f;
		FindPlayer();
	}

	if (abs(player->GetPosition().y - position.y) < 10)
	{
		direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
	}
	std::cout << Utils::MyMath::GetNormal(direction + up).x << ":" << Utils::MyMath::GetNormal(direction + up).y << std::endl;

	if (currentStatus == WindyPlaneStatus::WIND || currentStatus == WindyPlaneStatus::GUN)
	{
		if (!isAscending)
		{
			isAscending = true;
			windDirection = Utils::MyMath::GetNormal(direction + up);
		}

		windDirection.x < 0 ? SetFlipX(false) : SetFlipX(true);

		Translate(windDirection * currentSpeed * dt);
	}
	else
	{
		isAscending = false;

		direction.x < 0 ? SetFlipX(false) : SetFlipX(true);

		Translate(direction * currentSpeed * dt);
	}
}

void WindyPlane::AttackOneTwo() 
{
	currentStatus = WindyPlaneStatus::ONETWO;
}

void WindyPlane::AttackStraight()
{
	currentStatus = WindyPlaneStatus::STRAIGHT;
}

void WindyPlane::AttackUpperCut()
{
	currentStatus = WindyPlaneStatus::UPPERCUT;
}

void WindyPlane::AttackWind()
{
	currentStatus = WindyPlaneStatus::WIND;

	originalY = position.y;
	isAscending = true;
	isWindAttacking = true;

	// rangedAttackBox 내에 이펙트를 재생시켜야 함
}

//void WindyPlane::UpdateWindAttack(float dt)
//{
//	if (isAscending) 
//	{
//		
//	}
//	else
//	{
//		
//	}
//}

void WindyPlane::AttackGun()
{
	currentStatus = WindyPlaneStatus::GUN;

	// rangedAttackBox 내에 이펙트를 재생시켜야 함
}

void WindyPlane::AttackGunReady()
{
	currentStatus = WindyPlaneStatus::GUNREADY;
}

void WindyPlane::AttackOneTwoEvent()
{
	currentSpeed = speed;

	if (hitCount <= 2)
	{
		currentStatus = WindyPlaneStatus::IDLE;
	}
	// 이 부분 모두 다르게 구성 필요
	if (attackBox.getGlobalBounds().intersects(player->GetHitBox()) && !player->IsInvincible()) // 플레이어가 무적이 아닐 경우도 추가 필요
	{
		++hitCount;
		player->getHit = true;
	}

	CheckEndFrame();
}

void WindyPlane::AttackStraightEvent()
{
	currentSpeed = currentStatus == WindyPlaneStatus::STRAIGHT ? speed * 3.f : speed;

	ApplyAttackEvent(true);

}

void WindyPlane::AttackUpperCutEvent()
{
	currentSpeed = speed;

	ApplyAttackEvent(true);
}


void WindyPlane::AttackWindEvent()
{
	// 윈드 이펙트 표시 필요
	currentSpeed = speed;

	ApplyAttackEvent(true);
}

void WindyPlane::AttackGunEvent()
{
	// 총 이펙트 표시 필요
	currentSpeed = speed;

	ApplyAttackEvent(true);
}

void WindyPlane::AttackGunReadyEvent()
{
	int totalFrame = animator.GetCurrentClip()->GetTotalFrame() - 1;
	int currentFrame = animator.GetCurrentClipFrame();

	if (currentFrame == totalFrame)
	{
		currentStatus = WindyPlaneStatus::GUN;
	}
}

void WindyPlane::ApplyAttackEvent(bool isRanged = false)
{
	if ((isRanged ? rangedAttackBox : attackBox).getGlobalBounds().intersects(player->GetHitBox()) && !player->IsInvincible()) 
	{
		player->getHit = true;
	}

	CheckEndFrame();
}

void WindyPlane::FindPlayer()
{
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}

void WindyPlane::OnDamaged(float damage)
{
}

void WindyPlane::OnDamagedEvent()
{
	currentStatus = WindyPlaneStatus::IDLE;
}

void WindyPlane::OnDie()
{
	isAlive = false;
	currentStatus = WindyPlaneStatus::DEATH;
}

void WindyPlane::OnDieEvent()
{
	SetActive(false);
}

void WindyPlane::PlayAnimation(BossPartsStatus partsStatus, WindyPlaneStatus planeStatus)
{
	 //상태마다 재생 애니메이션이 바뀐다.

	if (hp <= maxHp * 0.2f)
	{
		currentPartsStatus = BossPartsStatus::NoArm;
		currentStatus = WindyPlaneStatus::FINAL;
	}
	else if (hp <= maxHp * 0.4f)
	{
		currentPartsStatus = BossPartsStatus::OneArm;
	}
	else if (hp <= maxHp * 0.6f)
	{
		currentPartsStatus = BossPartsStatus::NoProp;
	}
	else if (hp <= maxHp * 0.8f)
	{
		currentPartsStatus = BossPartsStatus::NoWing;
	}

	if (animator.GetCurrentClipId() != clipInfos[(int)currentPartsStatus].clips[(int)currentStatus])
	{
		currentClipId = clipInfos[(int)currentPartsStatus].clips[(int)currentStatus];

		animator.Play(currentClipId);
	}
}

void WindyPlane::CheckEndFrame()
{
	int totalFrame = animator.GetCurrentClip()->GetTotalFrame() - 1;
	int currentFrame = animator.GetCurrentClipFrame();

	if (currentFrame == totalFrame)
	{
		currentStatus = WindyPlaneStatus::IDLE;
	}

	if (currentStatus == WindyPlaneStatus::IDLE)
	{
		currentSpeed = speed;
	}
}