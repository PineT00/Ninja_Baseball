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
	bool isPicked;

public:
	Item::Types type = Item::Types::None;

	PickupItem(Types type, const std::string& name = "");
	~PickupItem() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPickUpAction(std::function<void(const std::string&)> action);
	bool IsPicked() { return isPicked; }
	void SetIsPicked(bool isPicked) { this->isPicked = isPicked; }
	const std::function<void(const std::string&)>& GetPickUpAction() const { return pickupAction; }
};

