#include "pch.h"
#include "WindyPlane.h"
#include "Player.h"

WindyPlane::WindyPlane(const std::string& name)
	: SpriteGo(name)
{
}

void WindyPlane::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);
}

void WindyPlane::Reset()
{
	animator.ClearEvent();
}

void WindyPlane::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
}

void WindyPlane::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
