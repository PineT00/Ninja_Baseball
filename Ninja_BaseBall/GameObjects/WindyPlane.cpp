#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"
#include "SceneDev1.h"
#include "WindEffect.h"
#include "rapidcsv.h"
#include "Stage1.h"

WindyPlane::WindyPlane(const std::string& name)
	: Enemy(name), data(BOSS_TABLE->Get(BossType::WINDYPLANE))
{
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	
	animator.SetTarget(&sprite);

	windEffects.resize(EFFECTS_COUNT);

	// Hit, Attack Box
	hitBox.setFillColor(sf::Color::Yellow);
	hitBox.setSize({ 140,160 });
	hitBox.setOrigin({ 35.f, 150.f });

	closeAttackBox.setFillColor(sf::Color::Red);
	closeAttackBox.setSize({ 500,80 }); 
	closeAttackBox.setOrigin({ 0.f, 150.f });

	uppercutAttackBox.setFillColor(sf::Color::Cyan);
	uppercutAttackBox.setSize({ 100,100 });
	uppercutAttackBox.setOrigin({ 0.f, 150.f });

	rangedAttackBox.setFillColor(sf::Color::Blue);
	rangedAttackBox.setSize({ 400, 120 });
	rangedAttackBox.setOrigin({ -300.f, 100.f });

	// 데이터 테이블로 가져온 클립 추가
	clipInfos.resize(PARTS_STATUS_COUNT + 1);

	for (int parts = 0; parts < PARTS_STATUS_COUNT; ++parts)
	{
		clipInfos[parts].partsStatus = static_cast<BossPartsStatus>(parts);

		for (int clipCount = 0; clipCount < data[clipInfos[parts].partsStatus].files.size(); ++clipCount)
		{
			clipInfos[parts].clips.emplace_back(data[clipInfos[parts].partsStatus].files[clipCount]); // 클립 추가
			bool status = parts == clipCount ? true : false;
			clipInfos[parts].clipStatus.emplace_back(status);
		}
	}

	// clipInfos[parts] : 상태별 모음
	// clipInfos[parts].clips : 상태별 클립 모음
	// clipInfos[parts].clipStatus : 상태별 클립 사용 여부
	enemyEffectAnimator.SetTarget(&enemyOnHit);
	enemyOnHit.setScale(2.5f,2.5f);

	LoadAllEvents();
}

void WindyPlane::Reset()
{
	// FindGo
	scene = dynamic_cast<SceneDev1*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<Player*>(scene->FindGameObject("Player"));

	maxHealth = 2000;
	health = maxHealth;

	SetOrigin(Origins::BC);

	for (int i = 0; i < EFFECTS_COUNT; ++i)
	{
		windEffects[i] = new WindEffect();
		windEffects[i]->Init();
		windEffects[i]->Reset();
		windEffects[i]->SetPosition({position.x, Utils::Random::RandomRange(position.y - 120, position.y) });
		windEffects[i]->SetSortLayer(1);
		scene->ResortGameObject(windEffects[i]);

		scene->AddGameObject(windEffects[i]);
	}
}

void WindyPlane::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
	enemyEffectAnimator.Update(dt);

	closeAttackBox.setPosition(position);
	rangedAttackBox.setPosition(position);
	hitBox.setPosition(position);
	uppercutAttackBox.setPosition(position);
	enemyOnHit.setPosition(hitBox.getPosition().x, hitBox.getPosition().y - 100.f); // 히트 이펙트용 스프라이트

	float dist = Utils::MyMath::Distance(player->GetPosition(), position);
	// HP 마다 상태가 바뀐다.
	if (InputManager::GetKeyDown(sf::Keyboard::Num1))
	{
		health = health - maxHealth * 0.2f;
	}

	//상태마다 재생 애니메이션이 바뀐다.
	if (health <= 0)
	{
		currentStatus = WindyPlaneStatus::DEATH;
		currentSpeed = 0.f;
	}
	else if (health <= maxHealth * 0.2f)
	{
		currentPartsStatus = BossPartsStatus::NoArm;
		currentStatus = WindyPlaneStatus::FINAL;
	}
	else if (health <= maxHealth * 0.4f)
	{
		currentPartsStatus = BossPartsStatus::OneArm;
	}
	else if (health <= maxHealth * 0.6f)
	{
		currentPartsStatus = BossPartsStatus::NoProp;
	}
	else if (health <= maxHealth * 0.8f)
	{
		currentPartsStatus = BossPartsStatus::NoWing;
	}

	switch (currentPartsStatus)
	{
	case BossPartsStatus::Wing:
		if (currentStatus == WindyPlaneStatus::IDLE && isAwake)
		{
			isAwake = false;
			AttackWind(dt);
		}
	case BossPartsStatus::NoWing:
	case BossPartsStatus::NoProp:
	{
		if (closeAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
		{
			statusTimer += dt;

			if (statusTimer >= statusInterval)
			{
				statusTimer = 0.f;
				hitCount = 0;

				if (currentStatus == WindyPlaneStatus::IDLE)
				{
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

	}
		break;
	case BossPartsStatus::OneArm:
		if (closeAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
		{
			statusTimer += dt;

			if (statusTimer >= statusInterval)
			{
				statusTimer = 0.f;

				if (dist <= 600)
				{
					currentStatus = WindyPlaneStatus::STRAIGHT;
				}
			}
		}

		break;
	case BossPartsStatus::NoArm:
		if (!player->isGrip)
		{
			currentSpeed = speed * 3;
		}
		Crying(dt);

		break;
	}

	if(!player->isGrip && currentStatus == WindyPlaneStatus::HOLD)
	{
		currentSpeed = speed;
	}

	if (dist > 30 && currentPartsStatus != BossPartsStatus::NoArm)
	{
		ChasePlayer(dt);
	}
	else
	{
		findTimer = 0.f;
	}

	// Clamp 부분
	SetPosition(Utils::MyMath::Clamp(position, scene->stage->groundBoundBoss.getGlobalBounds()));


	PlayAnimation(currentPartsStatus, currentStatus);
}

void WindyPlane::Draw(sf::RenderWindow& window)
{
	if (player->GetPosition().y < position.y)
	{
		SetSortLayer(1);
		player->SetSortLayer(0);

	}
	else
	{
		SetSortLayer(0);
		player->SetSortLayer(1);
	}
	
	scene->ResortGameObject(player);
	scene->ResortGameObject(this);

	SpriteGo::Draw(window);

	window.draw(closeAttackBox);
	window.draw(rangedAttackBox);
	window.draw(hitBox); 
	window.draw(uppercutAttackBox);
	window.draw(enemyOnHit);

	if (SCENE_MANAGER.GetDeveloperMode())
	{
		closeAttackBox.setFillColor(sf::Color::Red);
		rangedAttackBox.setFillColor(sf::Color::Blue);
		hitBox.setFillColor(sf::Color::Yellow);
		uppercutAttackBox.setFillColor(sf::Color::Cyan);
	}
	else
	{
		closeAttackBox.setFillColor(sf::Color::Transparent);
		rangedAttackBox.setFillColor(sf::Color::Transparent);
		hitBox.setFillColor(sf::Color::Transparent);
		uppercutAttackBox.setFillColor(sf::Color::Transparent);
	}
}

void WindyPlane::SetFlipX(bool flipX)
{
	SpriteGo::SetFlipX(flipX);

	if (flipX)
	{
		closeAttackBox.setScale({ scale.x, scale.y });
		rangedAttackBox.setScale({ scale.x, scale.y });
		hitBox.setScale({ scale.x, scale.y });
		uppercutAttackBox.setScale({ scale.x, scale.y });
	}
	else
	{
		closeAttackBox.setScale({ -scale.x, scale.y });
		rangedAttackBox.setScale({ -scale.x, scale.y });
		hitBox.setScale({ -scale.x, scale.y });
		uppercutAttackBox.setScale({ -scale.x, scale.y });
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

void WindyPlane::AttackWind(float dt)
{
	direction.x = Utils::MyMath::GetNormal(player->GetPosition() - position).x;

	currentStatus = WindyPlaneStatus::WIND;

	for (int i = 0; i < EFFECTS_COUNT; ++i)
	{
		windEffects[i]->Call();
	}
}

void WindyPlane::AttackGun()
{
	currentStatus = WindyPlaneStatus::GUN;

	// rangedAttackBox 내에 이펙트를 재생시켜야 함
}

void WindyPlane::AttackGunReady()
{
	currentStatus = WindyPlaneStatus::GUNREADY;
}

void WindyPlane::Crying(float dt)
{
	sf::FloatRect rect = scene->stage->groundBoundBoss.getGlobalBounds();

	if (position.x < rect.left + 10)
	{
		cryingDirection.x = 1.f;
	}
	if (position.x > rect.left + rect.width - 10)
	{
		cryingDirection.x = -1.f;
	}
	if (position.y < rect.top + 10)
	{
		cryingDirection.y = 1.f;
	}
	if (position.y > rect.top + rect.height - 10)
	{
		cryingDirection.y = -1.f;
	}

	Translate(cryingDirection * currentSpeed * dt);
}

void WindyPlane::AttackOneTwoEvent()
{
	currentSpeed = speed;

	if (hitCount <= 2)
	{
		currentStatus = WindyPlaneStatus::IDLE;
	}
	// 이 부분 모두 다르게 구성 필요
	if (closeAttackBox.getGlobalBounds().intersects(player->GetHitBox()) && !player->IsInvincible()) // 플레이어가 무적이 아닐 경우도 추가 필요
	{
		++hitCount;
		player->OnDamage(0,1,GetPosition().x);
	}

	CheckEndFrame();
}

void WindyPlane::AttackStraightEvent()
{
	currentSpeed = currentStatus == WindyPlaneStatus::STRAIGHT ? speed * 3.f : speed;

	ApplyAttackEvent(true, false);
}

void WindyPlane::AttackUpperCutEvent()
{
	currentSpeed = speed;

	ApplyAttackEvent(false, false);
}


void WindyPlane::AttackWindEvent()
{
	// 윈드 이펙트 표시 필요
	currentSpeed = speed;

	CheckEndFrame();
}

void WindyPlane::AttackGunEvent()
{
	// 총 이펙트 표시 필요
	currentSpeed = speed;

	ApplyAttackEvent(false, true);
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

void WindyPlane::ApplyAttackEvent(bool isClosed, bool isRanged)
{
	if ((isRanged ? rangedAttackBox : isClosed ? closeAttackBox : uppercutAttackBox).getGlobalBounds().intersects(player->GetHitBox()) && !player->IsInvincible())
	{
		player->OnDamage(0,1,GetPosition().x);
	}

	CheckEndFrame();
}

void WindyPlane::FindPlayer()
{
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}

void WindyPlane::OnDamage(int damage, int count)
{
	Enemy::OnDamage(damage, count);

	if (health <= 0)
	{
		OnDie();
		return;
	}

	direction.x /= 10.f;
	direction.y = 0.f;
	currentStatus = WindyPlaneStatus::DAMAGED;
	enemyEffectAnimator.Play("animations/Enemy/enemy_OnHit.csv");
}

void WindyPlane::OnDamagedEvent()
{
	currentStatus = WindyPlaneStatus::IDLE;
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}

void WindyPlane::OnDie()
{
	currentSpeed = 0.f;
	currentStatus = WindyPlaneStatus::DEATH;
}

void WindyPlane::OnDieEvent()
{
	isDead = true;
	SetActive(false);
}

void WindyPlane::HoldEvent()
{
	currentStatus = WindyPlaneStatus::IDLE;
}

void WindyPlane::LoadAllEvents()
{
	// Entire Event
	std::function<void()> holdEvent = std::bind(&WindyPlane::HoldEvent, this);
	for (int partsStatus = 0; partsStatus < clipInfos.size(); ++partsStatus)
	{
		for (int windyStatus = 0; windyStatus < clipInfos[partsStatus].clips.size(); ++windyStatus)
		{
			animator.AddEvent(clipInfos[(int)partsStatus].clips[windyStatus], 0, holdEvent);
		}
	}

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
	for (int i = 5; i <= 9; ++i)
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

	for (int i = 5; i <= 9; ++i)
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

	for (int i = 5; i <= 9; ++i)
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
	animator.AddEvent(clipInfos[(int)BossPartsStatus::NoArm].clips[(int)WindyPlaneStatus::DEATH], 6, deathEvent);

}

void WindyPlane::PlayAnimation(BossPartsStatus partsStatus, WindyPlaneStatus planeStatus)
{
	if (animator.GetCurrentClipId() != clipInfos[(int)currentPartsStatus].clips[(int)currentStatus])
	{
		currentClipId = clipInfos[(int)currentPartsStatus].clips[(int)currentStatus];

		animator.Play(currentClipId);
	}
}

void WindyPlane::HoldAction()
{
	currentStatus = WindyPlaneStatus::HOLD;
	currentSpeed = 0.f;
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