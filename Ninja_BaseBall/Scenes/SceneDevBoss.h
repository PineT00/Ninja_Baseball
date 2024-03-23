#pragma once
#include "Scene.h"

class WindyPlane;
class Player;

class SceneDevBoss : public Scene
{
protected:
	SceneDevBoss(const SceneDevBoss&) = delete;
	SceneDevBoss(SceneDevBoss&&) = delete;
	SceneDevBoss& operator=(const SceneDevBoss&) = delete;
	SceneDevBoss& operator=(SceneDevBoss&&) = delete;

	WindyPlane* windyPlane = nullptr;
	Player* player = nullptr;

	sf::Vector2f windowSize;
	sf::RectangleShape stageBound; // 실제 맵에서는 다르게 적용 필요

public:
	SceneDevBoss(SceneIDs id);
	~SceneDevBoss() override = default;

	void Init() override;
	void Release() override;
	virtual void Reset();
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window);

	GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);
};

