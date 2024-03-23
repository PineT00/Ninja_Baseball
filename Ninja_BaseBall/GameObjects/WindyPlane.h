#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player;
class SceneDevBoss;

class WindyPlane : public SpriteGo
{
protected:
	WindyPlane(const WindyPlane&) = delete;
	WindyPlane(WindyPlane&&) = delete;
	WindyPlane& operator=(const WindyPlane&) = delete;
	WindyPlane& operator=(WindyPlane&&) = delete;

	SceneDevBoss* sceneDevBoss = nullptr;
	Player* player = nullptr;
	Animator animator;

	std::vector<std::function<void()>> bossAttackPatterns;
	std::vector<std::function<void()>> bossMovePatterns;

	std::vector<std::string> clipIds;

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

	// 보스 움직임 패턴들


	// 보스 공격 패턴들
};