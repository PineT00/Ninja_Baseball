#include "pch.h"
#include "PickupItem.h"

PickupItem::PickupItem(Types type, const std::string& name)
	: Item(type, name)
{
}

void PickupItem::Init()
{
	Item::Init();
}

void PickupItem::Reset()
{
}

void PickupItem::Update(float dt)
{
	Item::Update(dt);
}

void PickupItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}

void PickupItem::SetPickUpAction(std::function<void(const std::string&)> action)
{
	pickupAction = action;
}
