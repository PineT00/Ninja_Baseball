#pragma once
#include "Scene.h"

class Tilemap;
class UiHUD;
class SpriteGo;
class TextGo;
class InputField;

class SceneDev1 : public Scene
{
protected :
	SceneDev1(const SceneDev1&)				= delete;
	SceneDev1(SceneDev1&&)					= delete;
	SceneDev1& operator=(const SceneDev1&)	= delete;
	SceneDev1& operator=(SceneDev1&&)		= delete;	
	
	sf::Vector2f windowSize;

public :
	SceneDev1(SceneIDs id);
	~SceneDev1() override = default;

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