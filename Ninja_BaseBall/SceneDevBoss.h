#pragma once
#include "Scene.h"
class SceneDevBoss : public Scene
{
protected:
	SceneDevBoss(const SceneDevBoss&) = delete;
	SceneDevBoss(SceneDevBoss&&) = delete;
	SceneDevBoss& operator=(const SceneDevBoss&) = delete;
	SceneDevBoss& operator=(SceneDevBoss&&) = delete;

	sf::Vector2f windowSize;

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

