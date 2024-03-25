#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class WindyPlane;
class Player;

class WindEffect : public SpriteGo
{
protected:
	WindEffect(const WindEffect&) = delete;
	WindEffect(WindEffect&&) = delete;
	WindEffect& operator=(const WindEffect&) = delete;
	WindEffect& operator=(WindEffect&&) = delete;

	Player* player = nullptr;
	WindyPlane* windyPlane = nullptr;
	Animator animator;

	sf::Vector2f direction;

	std::string animationClipId = "animations/Boss/NewAnim/effect/wind.csv";

	float durationTimer = 0.f;
	float duration = 3.f;
	float speed = 100.f;

	bool isCalled = false;
	bool isSetVelocity = false;

public:
	WindEffect(const std::string& name = "WindEffect");
	~WindEffect() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Call() { isCalled = true; }

	const sf::FloatRect GetGlobalBounds() const { return sprite.getGlobalBounds(); }
};