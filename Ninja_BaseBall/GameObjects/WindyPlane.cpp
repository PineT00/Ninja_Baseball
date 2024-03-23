#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"
#include "SceneDevBoss.h"

WindyPlane::WindyPlane(const std::string& name)
	: SpriteGo(name)
{
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	// 데이터 테이블화 필요
	// Phase1
	clipIds.push_back("animations/Boss/Phase1/bossIdle.csv");
	clipIds.push_back("animations/Boss/Phase1/bossDashPunchOneShot.csv");
	clipIds.push_back("animations/Boss/Phase1/bossShootGun.csv");
	clipIds.push_back("animations/Boss/Phase1/bossOnDamaged.csv");
	clipIds.push_back("animations/Boss/Phase1/bossPunchOneShot.csv");
	clipIds.push_back("animations/Boss/Phase1/bossPunchTwoShot");
	clipIds.push_back("animations/Boss/Phase1/bossWindShot.csv");


	// Phase2



	// FindGo
	sceneDevBoss = dynamic_cast<SceneDevBoss*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<Player*>(sceneDevBoss->FindGameObject("player"));
	

}

void WindyPlane::Reset()
{
	animator.ClearEvent();
	animator.Play(clipIds[0]);
	SetOrigin(Origins::BC);
}

void WindyPlane::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

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

	if (InputManager::GetKeyUp(sf::Keyboard::Num1))
	{
		if (animator.GetCurrentClipId() != clipIds[0])
			animator.Play(clipIds[0]);
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num2))
	{
		if (animator.GetCurrentClipId() != clipIds[1])

			animator.Play(clipIds[1]);
	}
	else if (InputManager::GetKeyUp(sf::Keyboard::Num3))
	{
		if (animator.GetCurrentClipId() != clipIds[2])

			animator.Play(clipIds[2]);
	}

	direction.x < 0 ? SetFlipX(true) : SetFlipX(false);

	Translate(direction * speed * dt);
}

void WindyPlane::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void WindyPlane::ChasePlayer()
{
}

void WindyPlane::PunchOneTime()
{
}

void WindyPlane::FindPlayer()
{
	direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
}
