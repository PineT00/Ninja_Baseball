#include "pch.h"
#include "Item.h"

Item::Item(Types itemType, const std::string& name)
	: SpriteGo(name)
{
}

void Item::Init()
{
	SpriteGo::Init();
}

void Item::Reset()
{
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
