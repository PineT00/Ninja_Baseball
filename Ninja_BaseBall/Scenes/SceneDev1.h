#pragma once
#include "Scene.h"

class Tilemap;
class UiHUD;
class SpriteGo;
class TextGo;
class InputField;
class YellowBaseBall;
class Stage1;
class Player;
class Player2;
class YellowBaseBall;
// class Enemy;

class SceneDev1 : public Scene
{
protected :
	SceneDev1(const SceneDev1&)				= delete;
	SceneDev1(SceneDev1&&)					= delete;
	SceneDev1& operator=(const SceneDev1&)	= delete;
	SceneDev1& operator=(SceneDev1&&)		= delete;	
	
	sf::RectangleShape cameraRect;

	float xMax = 0.f;

	Stage1* stage = nullptr;
	
	UiHUD* hud = nullptr;
	YellowBaseBall* yellowEnemy;
	sf::Vector2f windowSize;
public:
	Player* player = nullptr;
	Player2* player2 = nullptr;
	SceneDev1(SceneIDs id);
	~SceneDev1() override = default;

	sf::Vector2f ClampByTileMap(const sf::Vector2f point);

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