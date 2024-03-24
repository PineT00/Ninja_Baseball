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


	// Phase1

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

	//Wing
	std::function<void()> punchOneShotEvent = std::bind(&WindyPlane::PunchOneTimeEvent, this);
	animator.AddEvent(clipInfos[(int)BossPartsStatus::Wing].clips[0], 2, punchOneShotEvent);


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

	ChasePlayer(dt);

	switch (currentPartsStatus)
	{
	case BossPartsStatus::Wing:
		break;
	case BossPartsStatus::NoWing:
		break;
	case BossPartsStatus::NoProp:
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

	PlayAnimation(currentPartsStatus);
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

	direction.x < 0 ? SetFlipX(false) : SetFlipX(true);

	Translate(direction * speed * dt);
}

void WindyPlane::PunchOneTime()
{
	//currentStatus = Status::PUNCHONESHOT;
	// 2번 프레임에 맞는 포인트에 있으면 맞음
}

void WindyPlane::PunchTwoTime()
{
	//currentStatus = Status::PUNCHTWOSHOT;
}

void WindyPlane::GunAttack()
{
	//currentStatus = Status::GUNATTACK;

	if (rangedAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
	// rangedAttackBox 내에 이펙트를 재생시켜야 함
}

void WindyPlane::WindAttack()
{
	//currentStatus = Status::WINDATTACK;

	if (rangedAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
	// rangedAttackBox 내에 이펙트를 재생시켜야 함
}

void WindyPlane::Crying()
{

}

void WindyPlane::PunchOneTimeEvent()
{
	if (attackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
	//currentStatus = Status::CHASE;

	std::cout << "WING IDLE TEST" << std::endl;
}

void WindyPlane::PunchTwoTimeEvent()
{
	if (attackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
	
	if (!isTwice)
	{
		isTwice = true;
	}
	else
	{
		isTwice = false;
		//currentStatus = Status::CHASE;
	}
}

void WindyPlane::GunAttackEvent()
{
	// 프레임이 끝날 때 실행
	//currentStatus = Status::CHASE;
}

void WindyPlane::WindAttackEvent()
{
	// 프레임이 끝날 때 실행 
	//currentStatus = Status::CHASE;
}

void WindyPlane::CryingEvent()
{
	//currentStatus = Status::DEAD;
}

void WindyPlane::FindPlayer()
{
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}

void WindyPlane::OnDamaged(float damage)
{
}

void WindyPlane::OnDie()
{
}

void WindyPlane::PlayAnimation(BossPartsStatus status)
{
	currentClipInfo = clipInfos[(int)status];

	for (int i = 0; i < currentClipInfo.clips.size(); ++i)
	{
		if (currentClipInfo.clipStatus[i])
		{
			currentClipId = currentClipInfo.clips[i];
		}
	}

	if (animator.GetCurrentClipId() != currentClipId)
	{
		animator.Play(currentClipId);
	}
}