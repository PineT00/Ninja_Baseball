#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
public :
	enum class Types
	{
		None,
		GoldBat,
	};

	const int ITEM_TYPE_COUNT = 1;

protected:
	Item(const Item&) = delete;
	Item(Item&&) = delete;
	Item& operator=(const Item&) = delete;
	Item& operator=(Item&&) = delete;

public :
	Item(Types itemType, const std::string& name = "");
	~Item() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};