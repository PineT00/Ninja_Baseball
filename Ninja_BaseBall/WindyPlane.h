#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player;

class WindyPlane : public SpriteGo
{
protected:
	WindyPlane(const WindyPlane&) = delete;
	WindyPlane(WindyPlane&&) = delete;
	WindyPlane& operator=(const WindyPlane&) = delete;
	WindyPlane& operator=(WindyPlane&&) = delete;

	Animator animator;

	std::vector<std::function<void()>> bossAttackPatterns;
	std::vector<std::function<void()>> bossMovePatterns;

	sf::Vector2f windowSize;
	sf::Vector2f direction = { 1.f, 0.f };

	float speed = 200.f;

public:
	WindyPlane(const std::string& name = "windyplane");
	~WindyPlane() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window);

	// ���� ������ ���ϵ�


	// ���� ���� ���ϵ�
};