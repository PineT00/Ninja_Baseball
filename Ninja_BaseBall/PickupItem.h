#pragma once
#include "Item.h"
class PickupItem : public Item
{
protected:
	PickupItem(const PickupItem&) = delete;
	PickupItem(PickupItem&&) = delete;
	PickupItem& operator=(const PickupItem&) = delete;
	PickupItem& operator=(PickupItem&&) = delete;

	std::function<void(const std::string&)> pickupAction;

public:
	Item::Types type = Item::Types::None;

	PickupItem(Types type, const std::string& name = "");
	~PickupItem() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPickUpAction(std::function<void(const std::string&)> action);
	const std::function<void(const std::string&)>& GetPickUpAction() const { return pickupAction; }
};

