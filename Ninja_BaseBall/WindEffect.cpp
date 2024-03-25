#include "pch.h"
#include "WindEffect.h"
#include "WindyPlane.h"
#include "Player.h"

WindEffect::WindEffect(const std::string& name)
	: SpriteGo(name)
{
}

void WindEffect::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	windyPlane = dynamic_cast<WindyPlane*>(SCENE_MANAGER.GetScene(SceneIDs::SceneDevBoss)->FindGameObject("windyplane"));
	player = dynamic_cast<Player*>(SCENE_MANAGER.GetScene(SceneIDs::SceneDevBoss)->FindGameObject("player"));
}

void WindEffect::Reset()
{
	SetOrigin(Origins::BC);
	SetPosition({ windyPlane->GetPosition().x, Utils::Random::RandomRange(windyPlane->GetPosition().y, 120.f) });
	isSetVelocity = false;
}

void WindEffect::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

	if (isCalled)
	{
		if (animator.GetCurrentClipId() != animationClipId)
		{
			animator.Play(animationClipId);
		}

		if (!isSetVelocity)
		{
			isSetVelocity = true;
			direction = Utils::MyMath::GetNormal(player->GetPosition() - windyPlane->GetPosition());
			speed = Utils::Random::RandomRange(300, 500);
		}
		
		Translate(direction * speed * dt);

		durationTimer += dt;
		if (durationTimer > duration)
		{
			durationTimer = 0.f;

			SetActive(false);
			Reset();
			isCalled = false;
		}
	}

	if (player->GetHitBox().intersects(GetGlobalBounds()))
	{
		// ³Ëº¤
	}
}

void WindEffect::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
