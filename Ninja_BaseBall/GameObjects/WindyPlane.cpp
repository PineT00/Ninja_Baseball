#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"
#include "SceneDevBoss.h"
#include "rapidcsv.h"

WindyPlane::WindyPlane(const std::string& name)
	: SpriteGo(name)
{
	data = BOSS_TABLE->Get(BossType::WINDYPLANE);
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

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
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		true,false,false,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,true,false,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,true,false,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,true,false,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,false,true,false,false });
	clipInfos.push_back({ data.animationChaseClipId, data.animationPunchOneClipId, data.animationPunchTwoClipId, data.animationWindAttackClipId, data.animationGunAttackClipId,	data.animationCryingClipId,	data.animationDeadClipId,
		false,false,false,false,false,true,false });

	std::function<void()> punchOneShotEvent = std::bind(&WindyPlane::PunchOneTimeEvent, this);
	animator.AddEvent(data.animationPunchOneClipId, 2, punchOneShotEvent);

	// Phase2


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

	switch (currentStatus)
	{
	case WindyPlane::Status::CHASE:
	{
		if (attackBox.getGlobalBounds().intersects(player->GetHitBox()))
		{
			statusTimer += dt;
			if (statusTimer >= statusInterval)
			{
				statusTimer = 0.f;
				currentStatus = Status::PUNCHONESHOT;
			}
		}
	}
		break;
	case WindyPlane::Status::PUNCHONESHOT:
	{
		PunchOneTime();
	}
		break;
	case WindyPlane::Status::PUNCHTWOSHOT:
	{
		PunchTwoTime();
	}
		break;
	case WindyPlane::Status::WINDATTACK:
	{
		WindAttack();
	}
		break;
	case WindyPlane::Status::GUNATTACK:
	{
		GunAttack();
	}
		break;
	case WindyPlane::Status::CRYING:
	{
		Crying();
	}
		break;
	case WindyPlane::Status::DEAD:
	{

	}
		break;
	}

	// Test Code
	if (InputManager::GetKeyUp(sf::Keyboard::Num1))
	{
		currentStatus = Status::CHASE;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num2))
	{
		currentStatus = Status::PUNCHONESHOT;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num3))
	{
		currentStatus = Status::PUNCHTWOSHOT;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num4))
	{
		currentStatus = Status::WINDATTACK;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num5))
	{
		currentStatus = Status::GUNATTACK;
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num6))
	{
		currentStatus = Status::CRYING;
	}

	PlayAnimation(currentStatus);
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
		attackBox.setScale({ -scale.x, scale.y });
		rangedAttackBox.setScale({ -scale.x, scale.y });
		hitBox.setScale({ -scale.x, scale.y });
	}
	else
	{
		attackBox.setScale({ scale.x, scale.y });
		rangedAttackBox.setScale({ scale.x, scale.y });
		hitBox.setScale({ scale.x, scale.y });

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

	if (abs(player->GetPosition().y - position.y) < 20)
	{
		direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
	}

	direction.x < 0 ? SetFlipX(true) : SetFlipX(false);

	Translate(direction * speed * dt);
}

void WindyPlane::PunchOneTime()
{
	currentStatus = Status::PUNCHONESHOT;
	// 2번 프레임에 맞는 포인트에 있으면 맞음
}

void WindyPlane::PunchTwoTime()
{
	currentStatus = Status::PUNCHTWOSHOT;
}

void WindyPlane::GunAttack()
{
	currentStatus = Status::GUNATTACK;

	if (rangedAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
}

void WindyPlane::WindAttack()
{
	currentStatus = Status::WINDATTACK;

	if (rangedAttackBox.getGlobalBounds().intersects(player->GetHitBox()))
	{
		player->getHit = true;
	}
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
	currentStatus = Status::CHASE;
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
		currentStatus = Status::CHASE;
	}
}

void WindyPlane::GunAttackEvent()
{
	// 프레임이 끝날 때 실행
	currentStatus = Status::CHASE;
}

void WindyPlane::WindAttackEvent()
{
	// 프레임이 끝날 때 실행 
	currentStatus = Status::CHASE;
}

void WindyPlane::CryingEvent()
{
	currentStatus = Status::DEAD;
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

void WindyPlane::PlayAnimation(Status status)
{
	currentClipInfo = clipInfos[(int)status];

	if (currentClipInfo.isChase) currentClipId = currentClipInfo.chase;
	else if (currentClipInfo.isPunchOne) currentClipId = currentClipInfo.punchOne;
	else if (currentClipInfo.isPunchTwo) currentClipId = currentClipInfo.punchTwo;
	else if (currentClipInfo.isWindAttack) currentClipId = currentClipInfo.windAttack;
	else if (currentClipInfo.isGunAttack) currentClipId = currentClipInfo.gunAttack;
	else if (currentClipInfo.isCrying) currentClipId = currentClipInfo.crying;
	else if (currentClipInfo.isDead) currentClipId = currentClipInfo.dead;

	if (animator.GetCurrentClipId() != currentClipId)
	{
		animator.Play(currentClipId);
	}
}